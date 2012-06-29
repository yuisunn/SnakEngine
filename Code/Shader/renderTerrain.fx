   //  
  // Cached Procedural Textures 
 // Copyright (c) Carsten Dachsbacher
//

//
// TECHNIQUE: renderTerrain
// renders the terrain using the texture atlas. atmospheric effects are added.
//

float gammaControl, exposureControl;

// shadow and occlusion map textures + sampler
texture shadowTexture, occlusionTexture;

sampler shadowSampler    = sampler_state { Texture = ( shadowTexture ); MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };
sampler occlusionSampler = sampler_state { Texture = ( occlusionTexture ); MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };

texture atlasTexture;
sampler terrainMapAnisotropic = sampler_state 
{
    Texture   = ( atlasTexture );
	AddressU  = CLAMP;
	AddressV  = CLAMP;
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MAXANISOTROPY = 8;
};

//
// we use the atmosphere model by Naty Hofmann and Arcot Preetham (just a HLSL version of their implementation)
//

// constants for a fixed atmospheric condition
//一个固定的大气条件
float4	CV_CONSTANTS              = float4( 1.0, 1.4427, 0.5, 0.0 );
float4	CV_BETA_1_PLUS_2          = float4( 4.61e-5, 6.65e-5, 0.000114, 0.0 );
float4	CV_TERRAIN_REFLECTANCE    = float4( 0.014, 0.014, 0.014, 0.0 );
float4	CV_HG                     = float4( 0.00599, 1.997, 1.994, 0.0 );
float4	CV_BETA_DASH_1            = float4( 1.04e-6, 1.76e-6, 3.647e-6, 0.0 );
float4	CV_BETA_DASH_2            = float4( 6.67e-6, 8.67e-6, 1.25e-5, 0.0 );
float4	CV_ONE_OVER_BETA_1_PLUS_2 = float4( 21677.2, 15044.2, 8796.22, 0.0 );
float4	CV_TERM_MULTIPLIERS       = float4( 1.0, 0.7, 0.0, 0.0 );
float4	sunDirection              = float4( -0.438, 0.509, -0.741, 0.0 );
float4  sunColor                  = float4( 0.77, 0.70, 0.55, 100 );


struct SCATTERING
{
    float4 E, I;
};
//大气散射
SCATTERING	atmosphereScatter( float distance, float3 toCamera )
{
	SCATTERING coeff;

	// theta: angle between sun direction and view direction
	float4 r0_theta;

	r0_theta.x = dot( sunDirection, toCamera );
	r0_theta.y = r0_theta.x * r0_theta.x + 1.0f;

	// r0_theta = [cos(theta), 1+cos^2(theta), s] 

	// extinction term E
	float4 r1;

	r1 = exp2( -distance * CV_BETA_1_PLUS_2 );

	// apply reflectance to E to get total net effective E (total extinction)
	coeff.E = r1 * CV_TERRAIN_REFLECTANCE;

	// phase2(theta) = (1-g^2)/(1+g-2g*cos(theta))^(3/2)
	// CV_HG = [1-g^2, 1+g, 2g]
	float4 r4, r5;

	r4.x = rsqrt( CV_HG.z * r0_theta.x + CV_HG.y );
	r4.y = r4.x * r4.x;
	r4.x = r4.y * r4.x;
	r0_theta.w = r4.x * CV_HG.x;

	// inscattering (I) = (beta'_1 * phase_1(theta) + beta'_2 * phase_2(theta)) * 
	//                    [1-exp(-beta_1*s).exp(-beta_2*s)] / (beta_1 + beta_2)

	r4 = CV_BETA_DASH_1 * r0_theta.y;
	r5 = CV_BETA_DASH_2 * r0_theta.w;

	coeff.I  = r4 + r5;
	coeff.I *= 1.0f - r1;
	coeff.I *= CV_ONE_OVER_BETA_1_PLUS_2;
	coeff.I *= CV_TERM_MULTIPLIERS.y;

	coeff.I *= sunColor * sunColor.w;
	coeff.E *= sunColor * sunColor.w;

	return coeff;
}


struct FRAGMENT_UVC
{
    float4 position  : POSITION;
	float4 texture0  : TEXCOORD0;
	float4 texture1  : TEXCOORD1;
	float4 texture2  : TEXCOORD2;
};

FRAGMENT_UVC vsRenderTerrain( VERTEX_UV vertex )
{
    FRAGMENT_UVC result;

	float4 position = vertex.position;
	result.position = mul( matMVP, position );
	result.texture0 = vertex.texcoord;
	
	//地图集坐标
	// texture atlas coordinates
	result.texture0.xy -= 0.5f;
	result.texture0.xy *= ( rtSize.z - 2.0f * TILE_BORDER - 0.0f ) / rtSize.z;
	result.texture0.xy += 0.5f;

	result.texture0.xy *= texCoordBiasScale.zw;
	result.texture0.xy += texCoordBiasScale.xy;

	result.texture0.y = 1.0f - result.texture0.y;


	// distance to camera
	float3 toCamera = cameraPosition - vertex.position;
	float  distance = length( toCamera ) * 0.1f;
	toCamera = normalize( toCamera );

	SCATTERING scatter = atmosphereScatter( distance, toCamera );

	result.texture1 = scatter.I;
	result.texture2 = scatter.E;

	return result;
}


float4 psRenderTerrain( FRAGMENT_UVC fragment ) : COLOR
{
	float4 surfaceColor = tex2D( terrainMapAnisotropic, fragment.texture0 ) * 1.0f;

	// decode higher dynamic range by using alpha channel
	//动态范围内解码高度 用a通道
	surfaceColor.xyz = surfaceColor.xyz * surfaceColor.w * 4.0f;

	// apply atmospheric effects
	//大气效果
	surfaceColor = surfaceColor * fragment.texture2 + fragment.texture1;;

	// opt 1: exposure
	//surfaceColor = 1.0f - exp( -3.0f * surfaceColor );

	// opt 2: exposure + tone mapping
	surfaceColor = 1.0f - exp( exposureControl * surfaceColor );
	surfaceColor.xyz = pow( surfaceColor.xyz, gammaControl );

	return surfaceColor;
}

technique renderTerrain
{
    pass Pass0
    {
		ZENABLE  = TRUE;
		ZFUNC    = LESSEQUAL;
		ZWRITEENABLE = TRUE;
		CULLMODE = CCW;
		//FillMode = WIREFRAME;

		Sampler[ 0 ] = ( terrainMapAnisotropic );

		VertexShader = compile vs_1_1 vsRenderTerrain();
		PixelShader  = compile ps_2_0 psRenderTerrain();
    }
}


//
// TECHNIQUE: renderSky
// render a sky dome with the in-scattering term of the atmospheric model
//


FRAGMENT_UVC vsRenderSky( VERTEX_UV vertex )
{
    FRAGMENT_UVC result;

	// place sky dome
	float4 pos = 2.0f * vertex.position + cameraPosition;
	
	pos.w = 1.0f;
	result.position = mul( matMVP, pos );

	// we just need the inscattering term
	result.texture0 = 0.0f;
	
	// distance to camera
	float3 toCamera  = cameraPosition - pos;
	float  distance  = length( toCamera ) * 0.1f;
	toCamera = normalize( toCamera );

	SCATTERING scatter = atmosphereScatter( distance, toCamera );

	result.texture1 = scatter.I;
	result.texture2 = scatter.E;

	return result;
}

float4 psRenderSky( FRAGMENT_UVC fragment ) : COLOR
{
	// opt 1: exposure
	//return 1.0f - exp( -3.0f * fragment.texture1 );

	// opt 2: exposure + tone mapping
	return pow( 1.0f - exp( exposureControl * fragment.texture1 ), gammaControl );
}

technique renderSky
{
    pass Pass0
    {
		ZENABLE  = TRUE;
		ZFUNC    = LESSEQUAL;
		ZWRITEENABLE = FALSE;
		CULLMODE = CW;

		VertexShader = compile vs_1_1 vsRenderSky();
		PixelShader  = compile ps_2_0 psRenderSky();
    }
}
