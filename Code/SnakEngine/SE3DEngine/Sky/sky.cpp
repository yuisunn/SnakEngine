#include <Sky/sky.h>

void SkyBox::Init( LPSTR pFileName, LPDIRECT3DDEVICE9 pDevice )
{
 
    m_ver = NULL;
    m_pTexture = NULL;
	VERTEX *NP0 = NULL;

	int theta, phi;
	int radius = 1500;
	int dtheta = 15;	// 纬度
	int dphi = 15;		// 经度
	s_NumVertices = (int) ( (360/dtheta) * (90/dphi) * 4 );	// 顶点数量

	VERTEX *NP = new VERTEX[ s_NumVertices ]; 
	int n = 0; 
	for ( phi = 0; phi <= 90 - dphi; phi += (int)dphi ) 
	{
		for ( theta = 0; theta <= 360 - dtheta; theta += (int)dtheta ) 
		{ 
			NP[n].x = radius * sinf(phi * DTOR) * cosf(theta * DTOR); 
			NP[n].y = radius * sinf(phi * DTOR) * sinf(theta * DTOR);   
			NP[n].z = radius * cosf(phi * DTOR); 
			n++;
			NP[n].x = radius * sinf((phi + dphi) * DTOR) * cosf(theta * DTOR); 
			NP[n].y = radius * sinf((phi + dphi) * DTOR) * sinf(theta * DTOR); 
			NP[n].z = radius * cosf((phi + dphi) * DTOR); 
			n++; 
			NP[n].x = radius * sinf(phi * DTOR) * cosf((theta + dtheta) * DTOR); 
			NP[n].y = radius * sinf(phi * DTOR) * sinf((theta + dtheta) * DTOR); 
			NP[n].z = radius * cosf(phi * DTOR); 
			n++; 

			if (phi > -90 && phi < 90) 
			{ 
				NP[n].x = radius * sinf((phi + dphi) * DTOR) * cosf((theta + dtheta) * DTOR); 
				NP[n].y = radius * sinf((phi + dphi) * DTOR) * sinf((theta + dtheta) * DTOR); 
				NP[n].z = radius * cosf((phi + dphi) * DTOR); 
				n++; 
			} 
		} 
	}

	D3DXCreateTextureFromFile(pDevice, pFileName, &m_pTexture);

	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX)*s_NumVertices,0,MYP,D3DPOOL_DEFAULT,&m_ver,NULL)))
	{
		return;
	}
	
	m_ver->Lock(0,0,(void**)&NP0,0);

	for( int i=0; i<s_NumVertices; i++ )
	{
		NP0[i] = NP[i];
		NP0[i].color = 0xffffffff;
	}

	for( int i=0; i<s_NumVertices; i++ )
	{
		float vx = NP0[i].x;
		float vy = NP0[i].y;
		float vz = NP0[i].z;
		float mag = (float)sqrt( SQR(vx) + SQR(vy) + SQR(vz) );
		vx /= mag;
		vy /= mag;
		vz /= mag;
		NP0[i].v = 1.0f * (float)( atan2(vx, vz) / (D3DX_PI*2) ) + 0.5f;
		NP0[i].u = 1.0f * (float)( asin(vy) / D3DX_PI ) + 0.5f;
	}

	m_ver->Unlock();

	return;
}


SkyBox::~SkyBox()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_ver );
}


VOID SkyBox::Draw(LPDIRECT3DDEVICE9 pd3dDevice)
{
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	//
	//m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	//m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	//m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	//m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	pd3dDevice->SetStreamSource(0,m_ver,0,sizeof(VERTEX));
	pd3dDevice->SetFVF(MYP);
	pd3dDevice->SetTexture(0, m_pTexture);
	pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 574);
}


