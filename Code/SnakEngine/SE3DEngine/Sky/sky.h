#ifndef _SKY_
#define _SKY_
#include <d3dx9.h>
#include <Comm/Def.h>

#define DTOR (D3DX_PI / 180.0f) // 1度角度值对应的弧度值
#define MYP (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define SQR(x) (x*x)

typedef struct 
{ 
	float x, y, z; 
	DWORD color; 
	float u, v; 
} VERTEX;

class SkyBox
{
public:
	~SkyBox();
	void Init(LPSTR pFileName, LPDIRECT3DDEVICE9 pd3dDevice);
	void Draw(LPDIRECT3DDEVICE9 pd3dDevice);
private:
	LPDIRECT3DVERTEXBUFFER9 m_ver;
	LPDIRECT3DTEXTURE9		m_pTexture;
	int						NumVertices;
	VERTEX					*NP, *NP0;
	int s_NumVertices;

};



#endif