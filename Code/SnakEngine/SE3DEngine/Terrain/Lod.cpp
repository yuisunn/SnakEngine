   //  
  // Cached Procedural Textures 
 // Copyright (c) Carsten Dachsbacher
//

#include <SEScene/QuadTree.h>
#include "lod.h"
#include <Comm/Def.h>
#include <Scene/frustum.h>

int		heightMapX,			// resolution of the height map used as geometry
		heightMapY,
		heightMapXFull,		// full resolution of the height map (stored as texture)
		heightMapYFull;

// the maximum (virtual) texture resolution of the terrain is:
// [(heightMapX/quadsPerTile)*MAX_TILE_TEXTURE_SIZE]?

// the terrain is subdivided into smaller chunks. the smallest size is quadsPerTile*quadsPerTile heixels
//地图细分到较小的块 较小的尺寸是 blabalabal?
int		quadsPerTile = 8;

// scaling of the input terrain
static	float	hmEdgeLength = 0.018f;
float	terrainMaxElevation = 1000.0f*2.0f, terrainExtend = 7650.0f * 2.0f;

// this is the data for all terrain chunks
//地图块指针
TERRAINTILE	**terrainQT = NULL;
//地图块渲染
TERRAINTILE	**terrainQTRender = NULL; // and a list of the chunks to be rendered
int			qlodOffset[ 16 ], qlodNode[ 16 ], qlodNodes, qlodMaxLevel, qlodToRender;

// the texture atlas
//地图集
ATLAS	atlas[ ATLAS_TEXTURES ];

// recursive (quadtree) generation of the terrain-lod
//递归生成地形树
void	buildQLOD( IDirect3DDevice9* pD3DDevice, int node, int x, int y, int size, int level, WORD *heightMap16, LPDIRECT3DTEXTURE9 heightMapTexture16 )
{
	int	i, j;

	if ( level >= qlodMaxLevel ) return;

	TERRAINTILE *tile = new TERRAINTILE;

	//
	// setup texture stuff
	//
	tile->pHeightMapTexture = heightMapTexture16;
	tile->coordHM.x = (float)x / (float)heightMapX;
	tile->coordHM.y = (float)y / (float)heightMapX;
	tile->coordHM.z = (float)size / (float)heightMapX;
	tile->coordHM.w = (float)size / (float)heightMapX;
	tile->tileSize  = 0;
	tile->tileIdx   = -1;
	tile->worldSize = size;

	// use this, to take full geometric resolution everytime and everywhere
	//quadsPerTile = size;

	//
	// create input geometry
	//
	tile->fvfMeshVertex = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	tile->sizePerVertex = 6 * sizeof( float );

	DWORD creationFlags = D3DXMESH_VB_MANAGED | D3DXMESH_IB_MANAGED;

	//creationFlags = 0;
	if ( quadsPerTile * quadsPerTile * 2 * 3 >= 65535 )
		creationFlags |= D3DXMESH_32BIT;

	float *pData = NULL;
	//创建mesh 用来添加 顶点
	D3DXCreateMeshFVF(	(quadsPerTile+2) * (quadsPerTile+2) * 2,
						( quadsPerTile + 1+2 ) * ( quadsPerTile + 1+2 ),
						creationFlags,
						tile->fvfMeshVertex,
						pD3DDevice,
						&tile->pMesh );

	HRESULT hr;

	hr = tile->pMesh->LockVertexBuffer( D3DLOCK_DISCARD, (void**)&pData );

	tile->center  = Vector4( 0.0f, 0.0f, 0.0f );
	tile->aabbMin = Vector4( +1e37f, +1e37f, +1e37f );
	tile->aabbMax = Vector4( -1e37f, -1e37f, -1e37f );

	int	step = size / quadsPerTile;

	for ( int jj = -1; jj < quadsPerTile + 1+1; jj++ )
		for ( int ii = -1; ii < quadsPerTile + 1+1; ii++ )
		{
			int idx = (ii+1) + (jj+1) * ( quadsPerTile + 1+2 );

			if ( idx >= ( quadsPerTile + 1+2 ) * ( quadsPerTile + 1+2 ) )
				idx = idx;

			int i = max( 0, min( quadsPerTile, ii ) );
			int j = max( 0, min( quadsPerTile, jj ) );

			float xc = (float)( ( i * step + x ) - heightMapX / 2 ) / (float)(heightMapX) * terrainExtend;
			float zc = (float)( ( j * step + y ) - heightMapX / 2 ) / (float)(heightMapX) * terrainExtend;

			pData[ idx * 6 + 0 ] = xc;
			pData[ idx * 6 + 1 ] = (float)heightMap16[ min( heightMapX - 1, ( i * step + x ) ) + 
												   	   min( heightMapX - 1, ( j * step + y ) ) * heightMapX ] * 0.1f / 2.677777f;//65535.0f * terrainMaxElevation;

			pData[ idx * 6 + 2 ] = zc;

			pData[ idx * 6 + 3 ] = (float)i / (float)( quadsPerTile );
			pData[ idx * 6 + 4 ] = (float)j / (float)( quadsPerTile );

			// poor mans chunk-lod skirt ;-)
			if ( ii != i || j != jj )
			{
				pData[ idx * 6 + 1 ] -= terrainMaxElevation * (float)size / (float)heightMapX * 0.25f;
				pData[ idx * 6 + 5 ] -= terrainMaxElevation * (float)size / (float)heightMapX * 0.25f;
			}

			if ( ii == i && j == jj )
				tile->center += Vector4( xc, pData[ idx * 6 + 1 ], zc );
			tile->aabbMin = VectorMin(tile->aabbMin, (Vector4)pData[ idx * 6 + 0 ] );
			tile->aabbMax = VectorMax(tile->aabbMax, (Vector4)pData[ idx * 6 + 0 ] );
		}

	tile->pMesh->UnlockVertexBuffer();

	tile->center /= (float)( ( quadsPerTile + 1 ) * ( quadsPerTile + 1 ) );

	void *pIndexBuffer;
	tile->pMesh->LockIndexBuffer( D3DLOCK_DISCARD, &pIndexBuffer );

	//添加索引
	if ( creationFlags & D3DXMESH_32BIT )
	{
		DWORD *pIndex = (DWORD*)pIndexBuffer;

		for ( j = 0; j < quadsPerTile+2; j++ )
			for ( i = 0; i < quadsPerTile+2; i++ )
			{
				*( pIndex++ ) = i     + j     * ( quadsPerTile + 3 );
				*( pIndex++ ) = i     + (j+1) * ( quadsPerTile + 3 );
				*( pIndex++ ) = (i+1) + j     * ( quadsPerTile + 3 );
				*( pIndex++ ) = (i+1) + j     * ( quadsPerTile + 3 );
				*( pIndex++ ) = i     + (j+1) * ( quadsPerTile + 3 );
				*( pIndex++ ) = (i+1) + (j+1) * ( quadsPerTile + 3 );
			}
	} else
	{
		WORD *pIndex = (WORD*)pIndexBuffer;

		for ( j = 0; j < quadsPerTile+2; j++ )
			for ( i = 0; i < quadsPerTile+2; i++ )
			{
				*( pIndex++ ) = i     + j     * ( quadsPerTile + 3 );
				*( pIndex++ ) = i     + (j+1) * ( quadsPerTile + 3 );
				*( pIndex++ ) = (i+1) + j     * ( quadsPerTile + 3 );
				*( pIndex++ ) = (i+1) + j     * ( quadsPerTile + 3 );
				*( pIndex++ ) = i     + (j+1) * ( quadsPerTile + 3 );
				*( pIndex++ ) = (i+1) + (j+1) * ( quadsPerTile + 3 );
			}
	}
	tile->pMesh->UnlockIndexBuffer();
	//优化mesh 重新排序 
	tile->pMesh->OptimizeInplace( D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_STRIPREORDER, NULL, NULL, NULL, NULL );

	terrainQT[ node ] = tile;

	buildQLOD( pD3DDevice, qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 0, x,        y,        size/2, level+1, heightMap16, heightMapTexture16 );
	buildQLOD( pD3DDevice, qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 1, x+size/2, y,        size/2, level+1, heightMap16, heightMapTexture16 );
	buildQLOD( pD3DDevice, qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 2, x,        y+size/2, size/2, level+1, heightMap16, heightMapTexture16 );
	buildQLOD( pD3DDevice, qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 3, x+size/2, y+size/2, size/2, level+1, heightMap16, heightMapTexture16 );
}


//
// create chunk lod quadtree
//
//创建 lod4x树块
void	createQLOD( IDirect3DDevice9* pD3DDevice, int heightMapSize, WORD *heightMap16, LPDIRECT3DTEXTURE9 heightMapTexture16 )
{
	int i;

	qlodMaxLevel = (int)( logf( (float)heightMapX / (float)quadsPerTile ) / logf( 2.0f ) ) + 1;
	int	curOfs = 0, curNode = 1;
	qlodNode[ 0 ] = 1;
	qlodNodes = 1;
	qlodOffset[ 0 ] = 0;
	for ( i = 1; i < qlodMaxLevel; i++ )
	{
        qlodOffset[ i ] = qlodOffset[ i - 1 ] + qlodNode[ i - 1 ];	
		qlodNode[ i ] = qlodNode[ i - 1 ] * 4;
		qlodNodes += qlodNode[ i ];
	}

	terrainQT = new TERRAINTILE*[ qlodNodes ];
	terrainQTRender = new TERRAINTILE*[ qlodNodes ];
	qlodToRender = 0;

	buildQLOD( pD3DDevice, 0, 0, 0, heightMapSize-1, 0, heightMap16, heightMapTexture16 );

	for ( int i = 0; i < ATLAS_TEXTURES; i++ )
		atlas[ i ].layout = new QuadTree();
}

//
// free memory
//
void	destroyQLOD()
{
	freeAllTexturesQLOD( 0, 0 );

	for ( int i = 0; i < qlodNodes; i++ )
	{
		SAFE_RELEASE( terrainQT[ i ]->pMesh );
		SAFE_DELETE ( terrainQT[ i ] );
	}

	for ( int i = 0; i < ATLAS_TEXTURES; i++ )
		SAFE_DELETE( atlas[ i ].layout );
}

//
// render all marked chunks (contained in terrainQTRender)
//
void	renderQLOD( ID3DXEffect *pEffect )
{
	LPDIRECT3DTEXTURE9	curTexture = NULL;
	int					curTileSize = 0;


	for ( int a = 0; a < qlodToRender; a++ )
	{
		int commit = 0;

		TERRAINTILE *tile = terrainQTRender[ a ];
		//块尺寸
		if ( curTileSize != tile->tileSize )
		{
			curTileSize = tile->tileSize;
			D3DXVECTOR4	rtSize = D3DXVECTOR4( 0.0f, 0.0f, (float)tile->tileSize, 0.0f );
			pEffect->SetVector( "rtSize", &rtSize );
			commit ++;
		}
		//设置贴图
		if ( curTexture != tile->pTexture )
		{
			curTexture = tile->pTexture;
			pEffect->SetTexture( "atlasTexture", tile->pTexture );
			commit++;
		}
		//贴图缩放 和偏移 的贴图坐标
		pEffect->SetVector ( "texCoordBiasScale", ( D3DXVECTOR4 *)&tile->coordTile ); 

		pEffect->CommitChanges();

		tile->pMesh->DrawSubset( 0 );
	}
}

//
// frees the texture tiles of a terrain chunk (and its sub-chunks)
//
//释放资源
void	freeAllTexturesQLOD( int node, int level )
{
	if ( level >= qlodMaxLevel ) return;

	TERRAINTILE *tile = terrainQT[ node ];

	if ( tile->tileIdx != - 1 )
	{
		int atlasIdx = tile->tileIdx >> 24;

		atlas[ atlasIdx ].layout->treeFree( tile->tileSize, tile->tileIdx & 0xffffff );

		tile->tileIdx  = -1;
		tile->tileSize = 0;
		tile->pTexture = NULL;
	}

	freeAllTexturesQLOD( qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 0, level+1 );
	freeAllTexturesQLOD( qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 1, level+1 );
	freeAllTexturesQLOD( qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 2, level+1 );
	freeAllTexturesQLOD( qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 3, level+1 );
}


// this functions determines, which terrain chunks are intersecting the view frustum and determines
// the required geometric and texture resolution
static int updateAmount;

// 对地形 通过 frustum 裁剪
void	traverseQLOD(Vector4& camPos, Vector4 &camDst, int node, int level, Frustum* frustum)
{
	if ( level >= qlodMaxLevel ) return;

	if ( level == 0 ) 
	{
		updateAmount = 0;

		qlodToRender = 0;
		for ( int i = 0; i < ATLAS_TEXTURES; i++ )
			atlas[ i ].count = 0;
	}

	TERRAINTILE *tile = terrainQT[ node ];

	// frustum test
	tile->intersectFrustum = frustum->CheckBox( &tile->aabbMin, &tile->aabbMax );

	if ( !tile->intersectFrustum )
	{
		freeAllTexturesQLOD( node, level );
		return;
	}

	//
	// compute tile size
	//计算瓦片尺寸

	// at first min and max distance of the bounding box of a chunk
	float	minDistance = +1e37f,
			maxDistance = -1e37f;
	//碰撞盒子
	for ( int i = 0; i < 8; i++ )
	{
		Vector4	boxVertex;
		boxVertex.x = ((i>>0)&1) ? tile->aabbMin.x : tile->aabbMax.x;
		boxVertex.y = ((i>>1)&1) ? tile->aabbMin.y : tile->aabbMax.y;
		boxVertex.z = ((i>>2)&1) ? tile->aabbMin.z : tile->aabbMax.z;

		Vector4 toTile = boxVertex - (Vector4)camPos;
		Vector4 camDir = camDst - camPos;
		camDir.Normalize();

		float distance = Vector3Dot(camDir, toTile )[0];

		minDistance = min( minDistance, distance );
		maxDistance = max( maxDistance, distance );
	}

	minDistance = max( 10.0f, minDistance );

	float	tileDistance;
	
	Vector4 toTile = tile->center - (Vector4)camPos;
	tileDistance = max( 10.0f, toTile.Length());

	float	worldSpaceSize  = (float)tile->worldSize / (float)heightMapX * terrainExtend;
	float	screenSpaceSize = worldSpaceSize / tileDistance;

	// get power of two texture size
	//获得两个文理大小
	extern int qualitySelect;
	int		tileSize = (int)( screenSpaceSize * 32.0f * 32.0f * (float)(qualitySelect+1) * 0.1f );
	tileSize = 1 << ( (int)ceilf( ( logf( (float)tileSize ) / logf( 2.0f ) + 0.1f*0 ) ) );

	tileSize = max( ATLAS_MINIMUM_TILE_SIZE, tileSize );

	// this is NO correct estimation of a screen space error - more likely a guess for the upper bound of it ;-)
	//不确定的 估计屏幕空间 
	float	maxScreenSpaceError = 0.25f * terrainExtend * (float)tile->worldSize / (float)quadsPerTile / minDistance * 512.0f / (float)(heightMapX-1.0f);

	if ( level == qlodMaxLevel - 1 )
		tileSize = min( MAX_TILE_TEXTURE_SIZE, tileSize );

	if ( ( maxScreenSpaceError <= 2.5f && tileSize <= 128 ) || level == qlodMaxLevel - 1 )
	{
		// ok... assign texture atlas space:

		// this strategy assumes, that we get the texture atlas space we need...
		//分配贴图space 
		//得到贴图集空间要
		if ( tileSize != tile->tileSize )
		{
			Vector4	coordTile;
			int tileIdx, quality, texLevel;

			// uncomment this, to restrict the amount of texels generated per frame (no real use, but nice to watch)
/*			if ( updateAmount != 0 && updateAmount + tileSize * tileSize > 64*64 )
				goto addTile2List;
			updateAmount += tileSize * tileSize;
*/
			// free texture tile space (size has changed)
			//释放瓦片空间
			freeAllTexturesQLOD( node, level );

		nextTryForNode:

			// determine, in what atlas texture we want to put this tile (according to its size)
			//确定 地图集想放那个块
			int tileLevel = (int)( logf( (float)tileSize ) / logf( 2.0f ) );
			int tileAtlas = max( 0, min( ATLAS_TEXTURES-1, (tileLevel - 5)*ATLAS_TEXTURES/6 ) );

			if ( atlas[ tileAtlas ].layout->treeScan( tileSize, &texLevel, &tileIdx, &quality, &coordTile ) == -1 )
			{
				// we could not place it, where we wanted to
				tileIdx = -1;

				// let's try neighbor atlas textures
				//尝试邻居的地图集
				int tileAtlasTargeted = tileAtlas;
				for ( int test = 1; test < ATLAS_TEXTURES; test ++ )
				{
					tileAtlas = tileAtlasTargeted - test;
					if ( tileAtlas >= 0 )
						if ( atlas[ tileAtlas ].layout->treeScan( tileSize, &texLevel, &tileIdx, &quality, &coordTile ) != -1 )
							goto foundSpaceForNode;
					tileAtlas = tileAtlasTargeted + test;
					if ( tileAtlas < ATLAS_TEXTURES )
						if ( atlas[ tileAtlas ].layout->treeScan( tileSize, &texLevel, &tileIdx, &quality, &coordTile ) != -1 )
							goto foundSpaceForNode;
				}
			} 

			// we could not place this tile in any atlas texture... try a smaller tile size
			//
			if ( tileIdx == -1 && tileSize > ATLAS_MINIMUM_TILE_SIZE )
			{
				tileSize >>= 1;
				goto nextTryForNode;
			}

		foundSpaceForNode:;
			if ( tileIdx != - 1 )
			{
				// ok, got some space!
				atlas[ tileAtlas ].layout->treeAllocate( tileSize, tileIdx );

				tile->coordTile = coordTile;
				tile->tileIdx   = tileIdx | ( tileAtlas << 24 );
				tile->tileSize  = tileSize;

				atlas[ tileAtlas ].assign[ atlas[ tileAtlas ].count ++ ] = tile;
				tile->pTexture  = atlas[ tileAtlas ].texture;
			}

		}
addTile2List:
		// yes, this tile will be rendered!
		//开始渲染
		if ( tile->tileIdx != -1 )
			terrainQTRender[ qlodToRender ++ ] = tile;
	} else
	{
		traverseQLOD(  camPos, camDst, qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 0, level+1 );
		traverseQLOD(  camPos, camDst, qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 1, level+1 );
		traverseQLOD(  camPos, camDst, qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 2, level+1 );
		traverseQLOD(  camPos, camDst, qlodOffset[ level+1 ] + (node-qlodOffset[level]) * 4 + 3, level+1 );
	}
}
 