   //  
  // Cached Procedural Textures 
 // Copyright (c) Carsten Dachsbacher
//

float4x4 matMVP;

float4	cameraPosition, 
		cameraDirection;

// size of border for textures tiles (>=2)
//瓦片边线的大小
// this size has to be increased, if mip-mapping for the tiles is applied!!!
//这个尺寸增 mip 映射
#define	TILE_BORDER 4

// bias and scale for tile placement
//偏移和裁剪参数 瓦片的
float4	texCoordBiasScale, destCoordBiasScale;

// size of source heightmap
float	heightMapSize;

// size and inverse size of render target + up to two additional attributes
float4	rtSize;

// basecolor of terrain
float4	terrainBaseColor;

// surface layer attributes
//层属性
float4	constraintAltitude,
		constraintSlope,
		constraintAttrib,
		materialColor,
		noisePanning;

texture texHeightCoverage, texColor, texHeight, texCoverage;

// sampler of intermediate height/coverage textures
sampler heightCoverage = sampler_state {
    Texture   = ( texHeightCoverage );
	AddressU  = CLAMP;
	AddressV  = CLAMP;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};

// guess what
sampler colorMap = sampler_state {
    Texture   = ( texColor );
	AddressU  = CLAMP;
	AddressV  = CLAMP;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};


texture texHeightMap0;

// this is the sampler for the input height map, if we would use bilinear interpolation 
sampler heightMap0 = sampler_state {
    Texture   = ( texHeightMap0 );
	AddressU  = CLAMP;
	AddressV  = CLAMP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// ... and for bi-cubic interpolation
sampler heightMap1 = sampler_state {
    Texture   = ( texHeightMap0 );
	AddressU  = CLAMP;
	AddressV  = CLAMP;
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;
};


texture texNoiseMap0;
sampler noiseMap1 = sampler_state { Texture = ( texNoiseMap0 );	AddressU = MIRROR; AddressV = MIRROR; MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };

// some shared structs
struct VERTEX_UV
{
    float4 position	: POSITION;
    float4 texcoord : TEXCOORD0;
};

struct FRAGMENT_UV
{
    float4 position  : POSITION;
	float4 texture0  : TEXCOORD0;
};

struct FRAGMRT2
{
	float4 color[2] : COLOR;
};


//
// height encoding and decoding (float->3x 8bit and vice versa)
//

#define BS 255.0f

//高度压缩
float3 encodeHeight( float h )
{
	float3 r;

	h *= BS;

	r.x = floor( h );
	h  -= r.x;
	r.y = floor( h * BS );
	h  -= r.y / BS;
	r.z = floor( h * BS * BS );

	return r / BS;
}

//解码
float  decodeHeight( float3 h )
{
	return dot( h, float3( 1.0f, 1.0f/BS, 1.0f/BS/BS ) );
}

#include "renderTerrain.fx"
#include "step0_resample_height.fx"
#include "step1_compute_normals.fx"
#include "step2_add_layer.fx"

