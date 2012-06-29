   //  
  // Cached Procedural Textures 
 // Copyright (c) Carsten Dachsbacher
//


// texture and sampler for the normal map
texture texNormalMap0;
sampler normalMap0 = sampler_state {
    Texture   = ( texNormalMap0 );
	AddressU  = CLAMP;
	AddressV  = CLAMP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


//
// TECHNIQUE: addLayer
// adds one surface layer at a time (according to height/slope constraints) and modifies height values
//

struct FRAGMENT_AL
{
    float4 position  : POSITION;
	float4 texture0  : TEXCOORD0;
	float4 texture1  : TEXCOORD1;
	float4 texture2  : TEXCOORD2;
	float4 texture3  : TEXCOORD3;
};



FRAGMENT_AL vsAddLayer( VERTEX_TILE vertex )
{
    FRAGMENT_AL result;

	// place tile in rendertarget
	result.position = vertex.position;
	result.position.xy *= vertex.coordTile.zw;
	result.position.xy += vertex.coordTile.xy;
	result.position = result.position * 2.0f - 1.0f;
	result.position.zw = 1.0f;
	
	result.texture0 = vertex.position;
	result.texture0.xy *= vertex.coordTile.zw;
	result.texture0.xy += vertex.coordTile.xy;
	result.texture0.y = 1.0f - result.texture0.y;
	result.texture1.zw = float2( 0.0, 1.0 );

	// compute noise texture coordinates from "world space"-texcoord (=original heightmap coord)
	result.texture1 = vertex.position;

	result.texture1.xy -= 0.5f;
	result.texture1.xy *= vertex.position.z / (vertex.position.z - 2.0f * TILE_BORDER);
	result.texture1.xy += 0.5f;
	
	result.texture1.xy *= vertex.coordHM.zw;
	result.texture1.xy += vertex.coordHM.xy;
	
	result.texture1.y = 1.0f - result.texture1.y;
	result.texture1.zw = float2( 0.0, 1.0 );

	// keep this texture coordinates (lighting computation uses this vertex shader, too)
	result.texture2 = result.texture1;

	// more texture coordinates for multiple noise octaves
	result.texture3 = result.texture1 * 8.0f + noisePanning;
	result.texture1 = result.texture1 * 1.0f + noisePanning;

	return result;
}

float	getNoise( float4 tc )
{
	//return 0;
	return tex2D( noiseMap1, tc ).x;
}

FRAGMRT2 psAddLayerCoverage( FRAGMENT_AL fragment, float3 parentColor, float coverage )
{
	FRAGMRT2 result;

	float	altitude = decodeHeight( tex2D( heightCoverage, fragment.texture0 ).xyz );
	float	slope    = tex2D( normalMap0, fragment.texture0 ).w * 4.0f; 
	
	// get nice per pixel noise
	float	noiseValue;
	noiseValue  = getNoise( fragment.texture1 );
	noiseValue *= getNoise( fragment.texture3 );
	noiseValue -= 0.1f;
	noiseValue *= 2.0f;
	
	// per pixel noise, according to surface parameters
	float	perPixelNoise = ( noiseValue ) * 2.0f * constraintAttrib.y + ( constraintAttrib.x - 0.5f ) * 4.0f;

	// evaluate constraints	
	float	hatFuncA, hatFuncS;
	
		// altitude right = height * m(right) + n(right)
		float	alt_right = altitude * constraintAltitude.x + constraintAltitude.y;
		// altitude left  = height * m(left) + n(left)
		float	alt_left  = altitude * constraintAltitude.z + constraintAltitude.w;
		hatFuncA = saturate( min( alt_left, alt_right ) );

		// slope right = slope * m(right) + n(right)
		float	slo_right = slope * constraintSlope.x + constraintSlope.y;
		// slope left  = slope * m(left) + n(left)
		float	slo_left  = slope * constraintSlope.z + constraintSlope.w;
		hatFuncS = saturate( min( slo_left, slo_right ) );
		
	// resulting coverage of new texture layer
	float coverageAlone = saturate( hatFuncS * hatFuncA * perPixelNoise );
	
	// new joined coverage
	coverage = saturate( coverage * coverageAlone );

	// new surface color
	float4 surfaceColor = float4( lerp( parentColor, materialColor.xyz, coverage ), 0.0f );
	
	// output #0: surfaceColor
	result.color[ 0 ] = surfaceColor;
	
	// displacement
	altitude += 0.003125f * coverage * tex2D( noiseMap1, fragment.texture1 ).x * constraintAttrib.z;

	// output #1: new height and coverage
	result.color[ 1 ] = float4( encodeHeight( altitude ), coverage );

	return result;
}

FRAGMRT2 psAddLayer( FRAGMENT_AL fragment )
{
	float   coverage    = tex2D( heightCoverage, fragment.texture0 ).w;
	float3	parentColor = tex2D( colorMap, fragment.texture0 ).xyz;
	
	return	psAddLayerCoverage( fragment, parentColor, coverage );
}

technique addLayer
{
    pass Pass0
    {
		ZENABLE			= FALSE;
		ZFUNC			= ALWAYS;
		ZWRITEENABLE		= FALSE;
		CULLMODE		= NONE;

		VertexShader = compile vs_2_0 vsAddLayer();
		PixelShader  = compile ps_2_0 psAddLayer();
    }
}





//
// TECHNIQUE: computeLayerLighting
// computes the lighting of a layer given its normals and surface colors
//

float3	computeLighting( FRAGMENT_AL fragment, float4 color, float4 normal )
{
	float3	light	= max( 0, dot( normal.xyz, sunDirection.zyx ) );
	
	// sun color
	light *= float3( 3.52, 3.42, 2.99 );

	// shadow information and ambient occlusion
	float	shadow = tex2D( shadowSampler, fragment.texture2 ).x;
	float	ambientOcclusion = tex2D( occlusionSampler, fragment.texture2 ).x * 2 - 0.7;
	
	// combine light * shadow + ambient light
	return	color * ( light * shadow + ambientOcclusion * float3( 0.39, 0.47, 0.75 ) );
	
}


float4 psComputeLighting( FRAGMENT_AL fragment ) : COLOR0
{
	float4	normal	     = tex2D( normalMap0, fragment.texture0 ) * 2.0f - 1.0f;
	float4	surfaceColor = tex2D( colorMap,   fragment.texture0 );

	surfaceColor.xyz = computeLighting( fragment, surfaceColor, normal );
	
	// allow higher dynamic range by using alpha channel	
	float maxV = max( surfaceColor.x, max( surfaceColor.y, surfaceColor.z ) );
	maxV = max( 1.0f, maxV );
	surfaceColor.w = maxV / 4.0f;
	surfaceColor.xyz /= maxV;

	return surfaceColor;
}

technique computeLayerLighting
{
    pass Pass0
    {
		ZENABLE			= FALSE;
		ZFUNC			= ALWAYS;
		ZWRITEENABLE	= FALSE;
		CULLMODE		= NONE;

		Sampler[ 0 ] = ( normalMap0 );
		Sampler[ 1 ] = ( colorMap );

		VertexShader = compile vs_1_1 vsAddLayer();
		PixelShader  = compile ps_2_0 psComputeLighting();
    }
}
