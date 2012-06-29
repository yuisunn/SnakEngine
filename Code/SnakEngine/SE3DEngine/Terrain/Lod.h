//LOD
//�汾 
////////////////
#ifndef _LOD__H
#define _LOD__H
#include <SEMath/Vector4.h>
#include <windows.h>
#include <d3dx9.h>
// a terrain-tile represents a square region of the terrain (original or coarser representation)
//һ��������Ƭ ����һ����ͼ������������

typedef struct
{
	Vector4				    center,			// the center and AABB of this terrain part�������� ��aabb����
							aabbMin, 
							aabbMax;

	LPD3DXMESH				pMesh;				// mesh for this terrain part ���ε� mesh ����
	DWORD					fvfMeshVertex, 
							sizePerVertex;

	LPDIRECT3DTEXTURE9		pTexture;			// texture containing surface color
	Vector4				coordTile;			// bias + scale: 4 floats defining rectangle of corresponding texture area//��Ƭ����ͼ�����
	int						tileSize, 
							tileIdx;			// stores tile index (lower 24 bit) and texture atlas (upper 8 bit)
	int						worldSize;			// real size in world space
	int						intersectFrustum;	// guess what
	
	LPDIRECT3DTEXTURE9		pHeightMapTexture;	// ptr to source height map
	Vector4				coordHM;			// bias + scale to adress corresponding rectangle in the heightmap �߶�ͼ��Ӧ����ͼ����
}TERRAINTILE;

// number of atlas textures (each 512? 
#define ATLAS_TEXTURES	32

#define	MAX_TILE_TEXTURE_SIZE	512
#define ATLAS_MINIMUM_TILE_SIZE	32

// this is, what we store for each atlas texture
class FQTree;
typedef struct
{
	// quadtree for managing the tile placement �Ĳ���������Ƭ����
	QuadTree				*layout;
	// its d3d texture
	LPDIRECT3DTEXTURE9	texture;
	// pointers to the terrain tiles stored //��Ƭ�洢 
	TERRAINTILE			*assign[ (MAX_TILE_TEXTURE_SIZE/ATLAS_MINIMUM_TILE_SIZE)*(MAX_TILE_TEXTURE_SIZE/ATLAS_MINIMUM_TILE_SIZE) ];
	// number of tiles ��Ƭ����
	DWORD				count;
}ATLAS;

// see lod.cpp
extern int	heightMapX, 
			heightMapY,
			heightMapXFull,
			heightMapYFull,
			quadsPerTile;

void	createQLOD( int heightMapSize, WORD *heightMap16, LPDIRECT3DTEXTURE9 heightMapTexture16 );
void	traverseQLOD( Vector4 &camPos, Vector4 &camDst, int node, int level );
void	freeAllTexturesQLOD( int node, int level );
void	renderQLOD( ID3DXEffect *pEffect );
void	destroyQLOD();

#endif