   //  
  // Cached Procedural Textures 
 // Copyright (c) Carsten Dachsbacher
//

// sampler of input height map
sampler heightSampler = sampler_state {
    Texture   = ( texHeight );
	AddressU  = WRAP;
	AddressV  = WRAP;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};

// sampler of intermediate coverage texture (we may need different sources for height or coverage data)
//中间覆盖贴图的采样
sampler coverageSampler = sampler_state {
    Texture   = ( texCoverage );
	AddressU  = CLAMP;
	AddressV  = CLAMP;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};

// textures and samplers of the bi-cubic interpolation
//贴图采样 

texture texBicubicWeightMap03,
		texBicubicWeightMap47,
		texBicubicWeightMap8B,
		texBicubicWeightMapCF;
		
sampler bicubicWeight03 = sampler_state { Texture = ( texBicubicWeightMap03 ); AddressU = WRAP;	AddressV = WRAP; MipFilter = NONE; MinFilter = POINT; MagFilter = POINT; };
sampler bicubicWeight47 = sampler_state { Texture = ( texBicubicWeightMap47 ); AddressU = WRAP;	AddressV = WRAP; MipFilter = NONE; MinFilter = POINT; MagFilter = POINT; };
sampler bicubicWeight8B = sampler_state { Texture = ( texBicubicWeightMap8B ); AddressU = WRAP;	AddressV = WRAP; MipFilter = NONE; MinFilter = POINT; MagFilter = POINT; };
sampler bicubicWeightCF = sampler_state { Texture = ( texBicubicWeightMapCF ); AddressU = WRAP;	AddressV = WRAP; MipFilter = NONE; MinFilter = POINT; MagFilter = POINT; };

struct VERTEX_TILE
{
    float4 position	 : POSITION;
    float4 coordHM   : TEXCOORD0;
    float4 coordTile : TEXCOORD1;
    float2 scale     : TEXCOORD2;
};

struct FRAGMENT_UV8
{
    float4 position  : POSITION;
	float4 texture0  : TEXCOORD0;
	float4 texture1  : TEXCOORD1;
	float4 texture2  : TEXCOORD2;
	float4 texture3  : TEXCOORD3;
	float4 texture4  : TEXCOORD4;
	float4 texture5  : TEXCOORD5;
	float4 texture6  : TEXCOORD6;
	float4 texture7  : TEXCOORD7;
};

//
// TECHNIQUE: vsResampleHeight1to1
// resamples the input height field and outputs base color, interpolated height and initial coverage
// 插值高度和初始化覆盖
//

FRAGMENT_UV8 vsResampleHeightBorder( VERTEX_TILE vertex )
{
    FRAGMENT_UV8 result;

	// place tile in render target
	//渲染目标的 瓦片
	result.position = vertex.position;
	result.position.xy *= vertex.coordTile.zw;
	result.position.xy += vertex.coordTile.xy;
	result.position = result.position * 2.0f - 1.0f;
	result.position.zw = 1.0f;
	
	// adjust texture coordinates from source heightfield
	//调整贴图坐标 从 高度
	result.texture0 = vertex.position;

	result.texture0.xy -= 0.5f;
	result.texture0.xy *= vertex.position.z / (vertex.position.z - 2.0f * TILE_BORDER);
	result.texture0.xy += 0.5f;

	result.texture0.xy *= vertex.coordHM.zw;
	result.texture0.xy += vertex.coordHM.xy;

	result.texture0.y = 1.0f - result.texture0.y;
	
	// a translation, that bi-cubic interpolation is correctly located
	//
	const float C = -1.5;
	
	// lookups for the 4x4 grid of heightvalues	
	//查找4x4方格的 高度值
	result.texture1 = result.texture0 + float4(-1+C,+0+C,0,0) / heightMapSize;
	result.texture2 = result.texture0 + float4(-1+C,+1+C,0,0) / heightMapSize;
	result.texture3 = result.texture0 + float4(-1+C,+2+C,0,0) / heightMapSize;
	result.texture4 = result.texture0 + float4(+1+C,-1+C,0,0) / heightMapSize;
	result.texture5 = result.texture0 + float4(+1+C,+0+C,0,0) / heightMapSize;
	result.texture6 = result.texture0 + float4(+1+C,+1+C,0,0) / heightMapSize;
	result.texture7 = result.texture0 + float4(+1+C,+2+C,0,0) / heightMapSize;

	result.texture0 = result.texture0 + float4(-1+C,-1+C,0,0) / heightMapSize;

	return result;
}

FRAGMRT2 psResampleHeightBaseColor( FRAGMENT_UV8 fragment )
{
	FRAGMRT2 result;

	// output #0: terrain base color	
	result.color[ 0 ] = terrainBaseColor;

	// output #1: height and initial coverage

	// G16R16 heightmap with 2 stored heightvalues per pixel
	float4 h04 = float4( tex2D( heightMap1, fragment.texture0 ).xy, tex2D( heightMap1, fragment.texture4 ).xy );
	float4 h15 = float4( tex2D( heightMap1, fragment.texture1 ).xy, tex2D( heightMap1, fragment.texture5 ).xy );
	float4 h26 = float4( tex2D( heightMap1, fragment.texture2 ).xy, tex2D( heightMap1, fragment.texture6 ).xy );
	float4 h37 = float4( tex2D( heightMap1, fragment.texture3 ).xy, tex2D( heightMap1, fragment.texture7 ).xy );
	

	// fraction of height map texture coordinate
	// seems like there's a bug in some ATI 9700 mobility drivers:
	// using the frac(.) method results in weird texture lookups causing false interpolation in one corner
	// of a heixel. Choosing the second method and WRAP-texture lookups works fine...
//	float2 tc = frac( fragment.texture0.xy * heightMapSize );
	float2 tc = fragment.texture0.xy * heightMapSize - 1;

	// get bicubic interpolation weights
	//的到双立方插值权重
	float h;	
	float4 W0 = tex2D( bicubicWeight03, tc );
	float4 W1 = tex2D( bicubicWeight47, tc );
	float4 W2 = tex2D( bicubicWeight8B, tc );
	float4 W3 = tex2D( bicubicWeightCF, tc );


	// weighted sum to obtain height value
	//权重总和获得高度值
	h  = dot( h04, W0 );
	h += dot( h15, W1 );
	h += dot( h26, W2 );
	h += dot( h37, W3 );

	// this is the hardware supported bilinear interpolation... if you really want to see that
	//h = tex2D( heightMap0, fragment.texture0 ).x;

	h += -constraintAttrib.z*(
			(tex2D( noiseMap1, fragment.texture0*8).x-0.5f) * 4.0f+
			(tex2D( noiseMap1, fragment.texture0*32).x-0.5f) * 1.0f
			) * 0.025f;


	result.color[ 1 ].xyz = encodeHeight( h );
	
	// initial coverage
	result.color[ 1 ].w = 1.0f;
	
	return result;
}

technique resampleHeightBaseColor
{
    pass Pass0
    {
		ZENABLE			= FALSE;
		ZFUNC			= ALWAYS;
		ZWRITEENABLE	= FALSE;

		VertexShader = compile vs_1_1 vsResampleHeightBorder();
		PixelShader  = compile ps_2_0 psResampleHeightBaseColor();
    }
}




//
// TECHNIQUE: resampleHeightMapColor
// this is more or less a dummy technique, which copies color, height and initial coverage
// it is required to copy data from the texture, where new data should be written to.
// more height modifications like blurring could be added here
//

FRAGMENT_UV vsResampleHeight1to1( VERTEX_TILE vertex )
{
    FRAGMENT_UV result;

	// place tile in render target
	result.position = vertex.position;
	result.position.xy *= vertex.coordTile.zw;
	result.position.xy += vertex.coordTile.xy;
	result.position = result.position * 2.0f - 1.0f;
	result.position.zw = 1.0f;
	
	// adjust texture coordinates from source data
	result.texture0 = vertex.position;
	result.texture0.xy *= vertex.coordTile.zw;
	result.texture0.xy += vertex.coordTile.xy;
	result.texture0.y = 1.0f - result.texture0.y;

	return result;
}


FRAGMRT2 psResampleHeightMapColor( FRAGMENT_UV fragment )
{
	FRAGMRT2 result;

	// output #0: terrain color	
	result.color[ 0 ] = tex2D( colorMap, fragment.texture0 );
	
	// output #1: height and coverage
	result.color[ 1 ] = float4( 
			tex2D( heightSampler, fragment.texture0 ).xyz,
			tex2D( coverageSampler, fragment.texture0 ).w );

	return result;
}

technique resampleHeightMapColor
{
    pass Pass0
    {
		ZENABLE			= FALSE;
		ZFUNC			= ALWAYS;
		ZWRITEENABLE	= FALSE;

		VertexShader = compile vs_1_1 vsResampleHeight1to1();
		PixelShader  = compile ps_2_0 psResampleHeightMapColor();
    }
}
