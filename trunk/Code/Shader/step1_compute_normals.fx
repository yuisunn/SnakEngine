   //  
  // Cached Procedural Textures 
 // Copyright (c) Carsten Dachsbacher
//

//
// TECHNIQUE: computeNormals
// computes the normals and maximum derivatives in axis directions (=slope) for a given height map
//

struct FRAGMENT_CN
{
    float4 position  : POSITION;
	float4 texture0  : TEXCOORD0;
	float4 texture1  : TEXCOORD1;
	float4 texture2  : TEXCOORD2;
};

FRAGMENT_CN vsComputeNormals( VERTEX_TILE vertex )
{
    FRAGMENT_CN result;

	// place tile in rendertarget
	result.position = vertex.position;
	result.position.xy *= vertex.coordTile.zw;
	result.position.xy += vertex.coordTile.xy;
	result.position = result.position * 2.0f - 1.0f;
	result.position.zw = 1.0f;
	
	// 1 over texture resolution
	float delta = rtSize.y;

	// adjust source rectangle in texture
	result.texture0 = vertex.position;
	
	result.texture0.w = 1.0f;
	result.texture0.xy *= vertex.coordTile.zw;
	result.texture0.xy += vertex.coordTile.xy;
	result.texture0.y = 1.0f - result.texture0.y;
	
	result.texture0.x  += 0.5f * rtSize.y;
	result.texture0.y  += 0.5f * rtSize.y;

	// neighbor samples
	result.texture1  = result.texture0 + float4( delta, 0.0f, 0.0f, 0.0f );
	result.texture2  = result.texture0 + float4( 0.0f, delta, 0.0f, 0.0f );

	result.texture1.zw = vertex.scale.xy;
	
	return result;
}

float4 psComputeNormals( FRAGMENT_CN fragment ) : COLOR0
{
	float   height;

	height = decodeHeight( tex2D( heightMap1, fragment.texture0 ).xyz );

	// compute and store normal

	// edge length of height map grid
	float edgeLength = fragment.texture1.w;
	float3 a = float3( edgeLength, decodeHeight( tex2D( heightMap1, fragment.texture1 ).xyz ) - height, 0.0 );
	float3 b = float3( 0.0,	decodeHeight( tex2D( heightMap1, fragment.texture2 ).xyz ) - height, edgeLength );
	float3 nrml = cross( b, a );

	nrml = normalize( nrml );
	nrml = float3( -nrml.z, nrml.y, nrml.x );
	nrml = nrml * 0.5f + 0.5f;

	// slope

	// height scale
	float heightScale = fragment.texture1.z;
	float maxSlope = max( abs( a.y ), abs( b.y ) ) * heightScale * 0.25f;

	return float4( nrml.xyz, maxSlope );
}

technique computeNormals
{
    pass Pass0
    {
		Lighting		= FALSE;
		ZENABLE			= FALSE;
		ZFUNC			= ALWAYS;
		ZWRITEENABLE	= FALSE;

		Sampler[ 0 ] = ( heightMap1 );

		VertexShader = compile vs_1_1 vsComputeNormals();
		PixelShader  = compile ps_2_0 psComputeNormals();
    }
}
