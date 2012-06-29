

#include <Terrain/Terrain.h>
#include <Comm/Def.h>
#include <tchar.h>
#include <strsafe.h>
#include "WinDef.h"
#include "s.h"

#define LOWEST 100.0f
#define HIGHEST 140.0f

// 全局函数
// num必须是一个大于0的数，返回log2(num)的整数部分，小数部分将被截掉
// 例如，log2(512)返回 9, log2(514)和log2(1000)也将返回 9
UINT log2(UINT num);
// 先销毁该节点的子节点，再销毁该节点
//void deleteTreeNode( QuadTree* pNode );

QuadTree::QuadTree()
{
	m_pTLChildNode = NULL;
	m_pBLChildNode = NULL;
	m_pTRChildNode = NULL;
	m_pBRChildNode = NULL;
}

QuadTree::~QuadTree()
{
	RELEASE( m_pTLChildNode );
	RELEASE( m_pBLChildNode );
	RELEASE( m_pTRChildNode );
	RELEASE( m_pBRChildNode );
}

QuadTree::QuadTree(NODE nodePosSize)
{
	m_pParentNode = NULL;
	m_bIsLeaf = true;
	m_bReachAcceptableLevel = false;
	m_bTouched = false;
	m_bIsVisible = false;

	m_pTLChildNode = NULL;
	m_pBLChildNode = NULL;
	m_pTRChildNode = NULL;
	m_pBRChildNode = NULL;

	m_nodePosSize = nodePosSize;
}

void QuadTree::SplitNode()
{
	UINT sizeDivide2 = m_nodePosSize.size >> 1;
	UINT sizeDivide4 = sizeDivide2 >> 1;
	// 左上角子节点
	NODE nodePosSize( m_nodePosSize.x - sizeDivide4, m_nodePosSize.y - sizeDivide4, sizeDivide2 );
	m_pTLChildNode = new QuadTree( nodePosSize );
	m_pTLChildNode->m_pParentNode = this;
	m_pTLChildNode->m_level = m_level - 1;
	m_pTLChildNode->m_EnumValue = TOPLEFT;

	// 左下角子节点
	nodePosSize.y += sizeDivide2;
	m_pBLChildNode = new QuadTree( nodePosSize );
	m_pBLChildNode->m_pParentNode = this;
	m_pBLChildNode->m_level = m_level -1;
	m_pBLChildNode->m_EnumValue = BOTTOMLEFT;

	// 右下角子节点
	nodePosSize.x += sizeDivide2;
	m_pBRChildNode = new QuadTree( nodePosSize );
	m_pBRChildNode->m_pParentNode = this;
	m_pBRChildNode->m_level = m_level - 1;
	m_pBRChildNode->m_EnumValue = BOTTOMRIGHT;

	// 右上角子节点
	nodePosSize.y -= sizeDivide2;
	m_pTRChildNode = new QuadTree( nodePosSize );
	m_pTRChildNode->m_pParentNode = this;
	m_pTRChildNode->m_level = m_level -1;
	m_pTRChildNode->m_EnumValue = TOPRIGHT;
	
	// 分裂节点后，便不再是叶子节点
	m_bIsLeaf = false;
}

BOOL QuadTree::IsSplit()
{
	if( m_level > 0 )
		return TRUE;
	else
		return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
// implementation of  Terrain
Terrain::Terrain()
{
	m_pHeadNode = NULL;
	m_numNodeRendered = 0;
	m_elevArray = NULL;

	m_pVB = NULL;
	m_pIB = NULL;
	m_vertexArray = NULL;
	m_indexArray = NULL;
	m_lowTexture = NULL;
	m_highTexture = NULL;
}

//读取高度图和贴图
////////////////////
void Terrain::LoadMap( LPCTSTR strFileName )
{
	m_pHeadNode = NULL;
	m_numNodeRendered = 0;
	// 从文件中读取地形数据。。。。。( .m_heightMap文件，以float浮点形式读取)
	FILE* _pFile = fopen(strFileName, "rb");
	if(_pFile == NULL)
	{
	   printf("m_heightMap文件打开失败\n");
	   return;
	}
	m_heightMapY, m_heightMapX = 513;
	m_heightMapXFull,m_heightMapYFull = 2049;
	// 开辟二维高程数组,并以读取数据将其初始化
	m_heightMap16Full = new WORD[m_heightMapXFull * m_heightMapYFull];
	fread(m_heightMap16Full, 1, m_heightMapXFull*m_heightMapYFull*sizeof(WORD),_pFile );
	fclose(_pFile);

	
	m_heightMap16Low = new WORD[ m_heightMapX * m_heightMapY ];

	int step = (m_heightMapXFull-1) / (m_heightMapX-1);
	for ( int y = 0; y < m_heightMapY; y++ )
		for ( int x = 0; x < m_heightMapX; x++ )
		{
			m_heightMap16Low[ x + y * m_heightMapX ] = 
				m_heightMap16Full[ min( m_heightMapXFull-1, x * step ) + min( m_heightMapYFull-1, y * step ) * m_heightMapXFull ];
		}
	//创建贴图
	createSurfaceNodes();
}

Terrain::~Terrain()
{
	// 销毁头结点及其所有的子节点，否则造成内存溢出
	RELEASE(m_pHeadNode);//调用delete pNode会在pNode的析构函数中先释放其四个子节点

	// 释放高程数组
	if( m_elevArray != NULL )
	{
		for(UINT i=0;i<m_width;i++)
		{
			delete [] m_elevArray[i];
		}
		delete [] m_elevArray;
	}
	// 释放法线向量数组
	if( m_normalArray != NULL )
	{
		for(UINT i=0;i<m_width;i++)
		{
			delete [] m_normalArray[i];
		}
		delete [] m_normalArray;
	}
	// 释放纹理数组
	delete [] m_tuArray;
	delete [] m_tvArray;
}

void Terrain::SetDimesion( float width, float height/*地形的几何尺寸*/,D3DXVECTOR2 centerPt/*地形的中点位置*/ )
{
	m_fWidth = width;
	m_fHeight = height;
	m_centerPt = centerPt;

	m_GridWidth = m_fWidth/(m_width - 1);
	m_GridHeight = m_fHeight/(m_width - 1);

	// 开辟二维法线向量数组,并初始化
	m_normalArray = new D3DXVECTOR3*[m_width];
	for(UINT i=0;i<m_width;i++)
	{
		m_normalArray[i] = new D3DXVECTOR3[m_width];
	}
	// 二维法线向量数组初始化，不包括边界部分
	for(UINT i=0;i<m_width - 1;i++)
	{
		for(UINT j=0;j<m_width - 1;j++)
		{
			D3DXVECTOR3 vec[2];
			vec[0] = D3DXVECTOR3( 0, m_elevArray[i][j] - m_elevArray[i+1][j], 0 );
			vec[1] = D3DXVECTOR3( 0, m_elevArray[i][j] - m_elevArray[i][j+1], 0 );
			D3DXVec3Cross( &m_normalArray[i][j], &vec[0], &vec[1] );
			D3DXVec3Normalize( &m_normalArray[i][j], &m_normalArray[i][j] );
		}
	}
	// 地形边界处 法线向量的初始化
	for(UINT i=0;i<m_width;i++)
	{
		m_normalArray[i][m_width-1] = D3DXVECTOR3(0, 1, 0);
		m_normalArray[m_width-1][i] = D3DXVECTOR3(0, 1, 0);
	}
}

HRESULT Terrain::InitShader(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;
	LPD3DXBUFFER pShaderBuffer;
	// Create vertex shader
	if( FAILED( hr = D3DXCompileShaderFromFile( "..\\comm\\hlsl.vsd", NULL, NULL, "main", 
		"vs_2_0", 0/*D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION*//*Flag*/, &pShaderBuffer, NULL/*Error buffer*/, &m_pVConstTable )))
		return hr;
	if( FAILED( hr = pd3dDevice->CreateVertexShader((DWORD*)pShaderBuffer->GetBufferPointer(), &m_pVShader)))
		return hr;
	// Create pixel shader
	if( FAILED( hr = D3DXCompileShaderFromFile( "..\\comm\\hlsl.psd", NULL, NULL, "main", 
		"ps_2_0", 0/*Flag*/, &pShaderBuffer, NULL/*Error buffer*/, &m_pPConstTable )))
		return hr;
	if( FAILED( hr = pd3dDevice->CreatePixelShader((DWORD*)pShaderBuffer->GetBufferPointer(), &m_pPShader)))
		return hr;
	return hr;
}

void Terrain::InitQuadTree()
{
	m_pHeadNode = new QuadTree( NODE( (m_width-1) >> 1, (m_width-1) >> 1, m_width-1 ) );
	m_pHeadNode->m_level = log2( m_width - 1 ) - 1;
	GrowQuadTree(m_pHeadNode);
}


void Terrain::LoadTextureFromFile( LPDIRECT3DDEVICE9 pd3dDevice, LPCTSTR lowTexFile, LPCTSTR highTexFile )
{
	// 创建纹理
	if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, lowTexFile, &m_lowTexture )))
	{
		char strTexFileName[60];
		sprintf( strTexFileName, "..\\%s", lowTexFile );
		if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, strTexFileName, &m_lowTexture )))
			m_lowTexture = NULL;
	}
	if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, highTexFile, &m_highTexture )))
	{
		char strTexFileName[60];
		sprintf( strTexFileName, "..\\%s", highTexFile );
		if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, highTexFile, &m_highTexture )))
			m_highTexture = NULL;
	}
}

void Terrain::CreateTerrain( LPDIRECT3DDEVICE9 pd3dDevice, UserControl* pCamera )
{
	
	m_pCamera = pCamera;
	// 将根节点及其所有子节点的属性还原为false
	ClearTreeFlag(m_pHeadNode);
	
	// 遍历四叉树，判断节点的可见性和它的level是否达到要求，并对节点进行标记
	m_numNodeRendered = 0;
	m_frustum.Construct( pd3dDevice );
	
	ClimbQuadTree(m_pHeadNode, pd3dDevice);
	// 动态开辟顶点数组和索引数组
	UINT vertexArraySize = (m_numNodeRendered<<3) + m_numNodeRendered;  //m_numNodeRendered * 9
	UINT indexArraySize  = (m_numNodeRendered<<4) + (m_numNodeRendered<<3);   //m_numNodeRendered * 24
	m_vertexArray = new TVector[vertexArraySize];
	m_indexArray  = new WORD[indexArraySize];
	// 遍历四叉树，寻找树中m_bReachAcceptableLevel == true的节点，并将所有符合条件的节点写进顶点数组和索引数组中
	m_vertexIndex = 0;
	m_indexIndex  = 0;
	RefineNode(m_pHeadNode);
	// 创建顶点缓冲和索引缓冲,还需设置某些flag以提高性能
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	// 创建 vertex buffer
	if( FAILED( pd3dDevice->CreateVertexBuffer( vertexArraySize * sizeof(TVector),
        D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY/*Usage*/, D3DFVF_TVector, D3DPOOL_SYSTEMMEM/*D3DPOOL*/, &m_pVB, NULL )))
		return;
	void* pVertices;
	if( FAILED(m_pVB->Lock( 0/*offset*/, vertexArraySize * sizeof(TVector), (void**)&pVertices, 
		D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE/*Flag*/ )))
		return;
	memcpy( pVertices, m_vertexArray, vertexArraySize * sizeof(TVector) );
	m_pVB->Unlock();
	// 创建 index buffer
	if( FAILED( pd3dDevice->CreateIndexBuffer( indexArraySize *sizeof(WORD), 
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY/*Usage*/, D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM/*D3DPOOL*/, &m_pIB, NULL )))
		return;
	void* pIndices;
	if( FAILED(m_pIB->Lock( 0/*offset*/, indexArraySize * sizeof(WORD), (void**)&pIndices,
		D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE/*Flag*/ )))
		return;
	memcpy( pIndices, m_indexArray, indexArraySize * sizeof(WORD) );
	m_pIB->Unlock();
	// 创建 vertex buffer完毕，释放顶点数组
	delete [] m_vertexArray;
	// 创建 index buffer完毕，释放索引数组
	delete [] m_indexArray;
}


// 
void  Terrain::RenderTerrain( LPDIRECT3DDEVICE9 pd3dDevice, UserControl* pCamera, bool bFog )
{
	// pre rendering
	D3DXVECTOR4 lightDirVec(3.0f, 1.0f, 0, 0);


	Matrix4x4 proj_matrix = pCamera->GetProjection();
	Matrix4x4 world_matrix = pCamera->GetObjectMatrix();
	Matrix4x4 view_matrix = pCamera->GetViewMatrix();
	Matrix4x4 wvp_matrix = world_matrix * view_matrix * proj_matrix;
	Matrix4x4 wv_matrix = world_matrix * view_matrix;

	m_pVConstTable->SetMatrix(pd3dDevice, "mWorldViewProj", (D3DXMATRIX*)&wvp_matrix);
	m_pVConstTable->SetMatrix(pd3dDevice, "mWorldView", (D3DXMATRIX*)&wv_matrix);
	m_pVConstTable->SetFloat( pd3dDevice, "lowest", LOWEST);
	m_pVConstTable->SetFloat( pd3dDevice, "highest", HIGHEST);
	m_pVConstTable->SetFloat( pd3dDevice, "ambient", 0.5f);
	m_pVConstTable->SetVector(pd3dDevice, "lightDir", &lightDirVec);

	// set pixel shader constant table
	D3DXHANDLE hLowSmpl =  m_pPConstTable->GetConstantByName(NULL, "lowTexSmpl");
	D3DXHANDLE hHighSmpl = m_pPConstTable->GetConstantByName(NULL, "highTexSmpl");
	D3DXCONSTANT_DESC lowSmplDesc, highSmplDesc;
	UINT count;
	m_pPConstTable->GetConstantDesc(hLowSmpl, &lowSmplDesc, &count);
	m_pPConstTable->GetConstantDesc(hHighSmpl,&highSmplDesc,&count);

	// set low texture
	pd3dDevice->SetTexture(lowSmplDesc.RegisterIndex, m_lowTexture);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	// set high texture
	pd3dDevice->SetTexture(highSmplDesc.RegisterIndex, m_highTexture);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	// set fog
	D3DXVECTOR4 fogColor(0.18f, 0.2f, 0.7f, 1.0f);
	m_pVConstTable->SetFloat(pd3dDevice, "nearFog", 200.0f);
	m_pVConstTable->SetFloat(pd3dDevice, "farFog", 1000.0f);
	m_pPConstTable->SetVector(pd3dDevice, "fogColor", &fogColor);
	if(bFog)
		m_pPConstTable->SetFloat(pd3dDevice, "bFog", 1.0f);
	else
		m_pPConstTable->SetFloat(pd3dDevice, "bFog", 0.0f);

	// rendering
	pd3dDevice->SetFVF( D3DFVF_TVector );
	pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TVector) );
	pd3dDevice->SetVertexShader(m_pVShader);
	pd3dDevice->SetPixelShader(m_pPShader);
	pd3dDevice->SetIndices( m_pIB );
	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0/*base index index*/, 0,/*MinVertexIndex*/
		(m_numNodeRendered << 3 ) + m_numNodeRendered/*NumVertices*/,
		0/*StartIndex*/,
		(m_numNodeRendered << 3 )/*PrimitiveCount*/);
	// post rendering
	pd3dDevice->SetVertexShader(NULL);
	pd3dDevice->SetPixelShader(NULL);
}

// 给定X坐标和Y坐标，求出该点的地形高度  未完
float Terrain::GetTerrainElev(float x, float z)
{
	/*float elev;*/
	// 先求得对应于点 (x, z) 的相对坐标
	float xFloat, zFloat;
	xFloat = (x - m_centerPt.x) / m_GridWidth + ((m_width - 1)>>1);
	zFloat = (z - m_centerPt.y) / m_GridHeight + ((m_width - 1)>>1);
	int i, j;
	j = (int)xFloat;  // 取下整数
	i = (int)zFloat;  // 取下整数
	return m_elevArray[i][j];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下为私有成员函数
// 将头结点生长成一棵完全的二叉树
void Terrain::GrowQuadTree(QuadTree* pNode)
{
	if(pNode->IsSplit())
	{
		pNode->SplitNode();
		GrowQuadTree( pNode->m_pBLChildNode );
		GrowQuadTree( pNode->m_pTLChildNode );
		GrowQuadTree( pNode->m_pTRChildNode );
		GrowQuadTree( pNode->m_pBRChildNode );
	}
}


// 遍历四叉树，判断节点的可见性和它的level是否达到要求，并对节点进行标记
void Terrain::ClimbQuadTree( QuadTree* pNode, LPDIRECT3DDEVICE9 pd3dDevice )
{
	pNode->m_bTouched = true;
	if( IsVisible( pNode, pd3dDevice ) )// 如果头结点可见，对该节点进行处理，否则，忽略之
	{
		pNode->m_bIsVisible = true;
		if( IsReachAcceptableLevel( pNode )||pNode->m_bIsLeaf )
		{// 如果该节点的level已足够低，或者该节点是叶子节点，不再往下遍历
			pNode->m_bReachAcceptableLevel = true;
			// 下面这句话要求每个节点只能被扩展一次
			m_numNodeRendered ++;
		}
		else
		{// 如果该节点的level还不够低，并且不是叶子节点，继续往下扩展遍历
			// 往下扩展遍历之前，确认它的四个相邻顶点的父节点被扩展遍历过
			pNode->m_bReachAcceptableLevel = false;
			QuadTree* pAdjNodeArray[4];
			GetAdjNodeParent4( pAdjNodeArray, pNode );
			for(int i=0;i<4;i++)
			{
				if( pAdjNodeArray[i] != NULL && pAdjNodeArray[i]->m_bTouched == false )
					ClimbQuadTree( pAdjNodeArray[i], pd3dDevice );
			}
			if( false == pNode->m_pBLChildNode->m_bTouched )
				ClimbQuadTree( pNode->m_pBLChildNode, pd3dDevice );
			if( false == pNode->m_pTLChildNode->m_bTouched )
				ClimbQuadTree( pNode->m_pTLChildNode, pd3dDevice );
			if( false == pNode->m_pBRChildNode->m_bTouched )
				ClimbQuadTree( pNode->m_pBRChildNode, pd3dDevice );
			if( false == pNode->m_pTRChildNode->m_bTouched )
				ClimbQuadTree( pNode->m_pTRChildNode, pd3dDevice );
		}
	}
	else
	{
		pNode->m_bIsVisible = false;
		pNode->m_bReachAcceptableLevel = false;
	}
}

// 得到节点的四个相邻节点的父节点，将它们的指针存储在一个数组里
void Terrain::GetAdjNodeParent4(QuadTree** pAdjNodeArray, QuadTree* pNode)
{
	NODE posSize = pNode->GetPosSize();
	// 查找该节点上面的那个节点的父节点
	pAdjNodeArray[0] = FindParentNode( posSize.x, posSize.y - posSize.size );
	// 查找该节点下面的那个节点的父节点
	pAdjNodeArray[1] = FindParentNode( posSize.x, posSize.y + posSize.size );
	// 查找该节点左边的那个节点的父节点
	pAdjNodeArray[2] = FindParentNode( posSize.x - posSize.size, posSize.y );
	// 查找该节点右边的那个节点的父节点
	pAdjNodeArray[3] = FindParentNode( posSize.x + posSize.size, posSize.y );
}

// 查找相对位置为(x, y)的那个节点的父节点
QuadTree* Terrain::FindParentNode(UINT x, UINT y)
{
	QuadTree* pNode = m_pHeadNode;
	while(1)
	{
		if( pNode == NULL )
			return NULL;
		NODE posSize = pNode->GetPosSize();
		if( posSize.x == x )
			return pNode->m_pParentNode; // 找到该节点，返回它的父节点
		else if( posSize.x < x )
		{
			if( posSize.y < y )
				pNode = pNode->m_pBRChildNode;
			else //posSize.y > y
				pNode = pNode->m_pTRChildNode;
		}
		else //posSize.x > x
		{
			if( posSize.y < y )
				pNode = pNode->m_pBLChildNode;
			else //posSize.y > y
				pNode = pNode->m_pTLChildNode;
		}
	}
}
// 查找相对位置为(x, y)的那个节点
QuadTree* Terrain::FindNode(UINT x, UINT y)
{
	QuadTree* pNode = m_pHeadNode;
	while(1)
	{
		if( pNode == NULL )
			return NULL;
		NODE posSize = pNode->GetPosSize();
		if( posSize.x == x )
			return pNode; // 找到该节点，返回它的父节点
		else if( posSize.x < x )
		{
			if( posSize.y < y )
				pNode = pNode->m_pBRChildNode;
			else //posSize.y > y
				pNode = pNode->m_pTRChildNode;
		}
		else //posSize.x > x
		{
			if( posSize.y < y )
				pNode = pNode->m_pBLChildNode;
			else //posSize.y > y
				pNode = pNode->m_pTLChildNode;
		}
	}
}

// 判断某个节点是否可见

BOOL Terrain::IsVisible(QuadTree* pNode, LPDIRECT3DDEVICE9 pd3dDevice)
{
	NODE posSize = pNode->GetPosSize();
	D3DXVECTOR3 centerPos;
	centerPos.x = ( int(posSize.x - ((m_width - 1)>>1)) ) * m_GridWidth + m_centerPt.x;
	centerPos.y = m_elevArray[posSize.y][posSize.x];
	centerPos.z = ( int(posSize.y - ((m_width -1 )>>1)) ) * m_GridHeight + m_centerPt.y;
	float radius = float((int)(posSize.size) * m_GridWidth)/1.414f;
	return m_frustum.CheckSphere(centerPos, radius );
}

// 判断某个节点的level是否已足够低
BOOL Terrain::IsReachAcceptableLevel(QuadTree* pNode)
{
	// 先求取节点中心的绝对坐标值
	NODE posSize = pNode->GetPosSize();
	Vector4 centerPos;
	centerPos.x = ( int(posSize.x - ((m_width - 1)>>1)) ) * m_GridWidth + m_centerPt.x;
	centerPos.y = m_elevArray[posSize.y][posSize.x];
	centerPos.z = ( int(posSize.y - ((m_width -1 )>>1)) ) * m_GridHeight + m_centerPt.y;
	// 再获取眼睛的绝对坐标值
	Vector4 eyePt = m_pCamera->GetCameraPosition();
	// 求取眼睛与节点中心之间的绝对距离

	Vector4 vec3 = centerPos - eyePt;

	if( vec3.Length()  > float(posSize.size << 4) )
		return TRUE;
	else
		return FALSE;
}

// 遍历四叉树，寻找树中m_bReachAcceptableLevel == true的节点，并将所有符合条件的节点写进顶点数组和索引数组中
// 每个节点由8个三角形组成，9个顶点数据，24个索引数据
void Terrain::RefineNode(QuadTree* pNode)
{
	if( pNode->m_bIsVisible )
	{
		if( pNode->m_bReachAcceptableLevel/* || !pNode->m_bIsVisible*//*for debug*/)
		{// 节点的细节已足够，将它送入顶点数组和索引数组
			// 顶点数组赋值，   0 1 2
			//                  3 4 5 
			//                  6 7 8
			// 计算顶点的位置向量和纹理坐标
			NODE posSize = pNode->GetPosSize();
			UINT sizeDivide2 = posSize.size >> 1;
			float top = ( int(posSize.y - sizeDivide2 - ((m_width -1 )>>1)) ) * m_GridHeight + m_centerPt.y;
			float bottom = ( int(posSize.y + sizeDivide2 - ((m_width -1 )>>1)) ) * m_GridHeight + m_centerPt.y;
			float left = ( int(posSize.x - sizeDivide2 - ((m_width -1 )>>1)) ) * m_GridWidth + m_centerPt.x;
			float right = ( int(posSize.x + sizeDivide2 - ((m_width -1 )>>1)) ) * m_GridWidth + m_centerPt.x;
			float centerX = ( int(posSize.x - ((m_width -1 )>>1)) ) * m_GridWidth + m_centerPt.x;
			float centerY = ( int(posSize.y - ((m_width -1 )>>1)) ) * m_GridWidth + m_centerPt.y;
			// 左上角顶点
			m_vertexArray[m_vertexIndex + 0].position.x = left;
			m_vertexArray[m_vertexIndex + 0].position.z = top;
			m_vertexArray[m_vertexIndex + 0].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 0].normal   = m_normalArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2];
			// for debug
			//m_vertexArray[m_vertexIndex + 0].position.y = 0;
			m_vertexArray[m_vertexIndex + 0].tu = m_tuArray[posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 0].tv = m_tvArray[posSize.y - sizeDivide2];

			// 上边顶点
			// 判断它上方的那个相邻节点的 m_bTouched 属性是否为true,如为真从数组中读取高程值，否则取相邻顶点高程值的平均值
			m_vertexArray[m_vertexIndex + 1].position.x = centerX;
			m_vertexArray[m_vertexIndex + 1].position.z = top;
			QuadTree* pAdjNode = FindNode( posSize.x, posSize.y - posSize.size );
			if( pAdjNode != NULL )
			{
				if( pAdjNode->m_bTouched )
				{
					m_vertexArray[m_vertexIndex + 1].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x];
					m_vertexArray[m_vertexIndex + 1].normal   = m_normalArray[posSize.y - sizeDivide2][posSize.x];
				}
				else
				{
					//m_vertexArray[m_vertexIndex + 1].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x];
					m_vertexArray[m_vertexIndex + 1].position.y = ( m_elevArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2]
						+ m_elevArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2])/2;
					m_vertexArray[m_vertexIndex + 1].normal = ( m_normalArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2]
						+ m_normalArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2])/2;
				}
			}
			else
			{
				m_vertexArray[m_vertexIndex + 1].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x];
				m_vertexArray[m_vertexIndex + 1].normal   = m_normalArray[posSize.y - sizeDivide2][posSize.x];
			}
			
			// for debug
			//m_vertexArray[m_vertexIndex + 1].position.y = 0;
			m_vertexArray[m_vertexIndex + 1].tu = m_tuArray[posSize.x];
			m_vertexArray[m_vertexIndex + 1].tv = m_tvArray[posSize.y - sizeDivide2];
			// 右上角顶点
			m_vertexArray[m_vertexIndex + 2].position.x = right;
			m_vertexArray[m_vertexIndex + 2].position.z = top;
			m_vertexArray[m_vertexIndex + 2].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 2].normal   = m_normalArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2];
			// for debug
			//m_vertexArray[m_vertexIndex + 2].position.y = 0;
			m_vertexArray[m_vertexIndex + 2].tu = m_tuArray[posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 2].tv = m_tvArray[posSize.y - sizeDivide2];

			// 左边顶点
			// 判断它左方的那个相邻节点的 m_bTouched 属性是否为true,如为真从数组中读取高程值，否则取相邻顶点高程值的平均值
			m_vertexArray[m_vertexIndex + 3].position.x = left;
			m_vertexArray[m_vertexIndex + 3].position.z = centerY;
			pAdjNode = FindNode( posSize.x - posSize.size, posSize.y );
			if( pAdjNode != NULL )
			{
				if( pAdjNode->m_bTouched )
				{
					m_vertexArray[m_vertexIndex + 3].position.y = m_elevArray[posSize.y][posSize.x - sizeDivide2];
					m_vertexArray[m_vertexIndex + 3].normal   = m_normalArray[posSize.y][posSize.x - sizeDivide2];
				}
				else
				{
					//m_vertexArray[m_vertexIndex + 3].position.y = m_elevArray[posSize.y][posSize.x - sizeDivide2];
					m_vertexArray[m_vertexIndex + 3].position.y = ( m_elevArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2]
						+ m_elevArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2])/2;
					m_vertexArray[m_vertexIndex + 3].normal = ( m_normalArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2]
						+ m_normalArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2])/2;
				}
			}
			else
			{
				m_vertexArray[m_vertexIndex + 3].position.y = m_elevArray[posSize.y][posSize.x - sizeDivide2];
				m_vertexArray[m_vertexIndex + 3].normal   = m_normalArray[posSize.y][posSize.x - sizeDivide2];
			}
			
			// for debug
			//m_vertexArray[m_vertexIndex + 3].position.y = 0;
			m_vertexArray[m_vertexIndex + 3].tu = m_tuArray[posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 3].tv = m_tvArray[posSize.y];

			// 中心顶点
			m_vertexArray[m_vertexIndex + 4].position.x = centerX;
			m_vertexArray[m_vertexIndex + 4].position.z = centerY;
			m_vertexArray[m_vertexIndex + 4].position.y = m_elevArray[posSize.y][posSize.x];
			m_vertexArray[m_vertexIndex + 4].normal   = m_normalArray[posSize.y][posSize.x];
			// for debug
			//m_vertexArray[m_vertexIndex + 4].position.y = 0;
			m_vertexArray[m_vertexIndex + 4].tu = m_tuArray[posSize.x];
			m_vertexArray[m_vertexIndex + 4].tv = m_tvArray[posSize.y];

			// 右边顶点
			// 判断它左方的那个相邻节点的 m_bTouched 属性是否为true,如为真从数组中读取高程值，否则取相邻顶点高程值的平均值
			m_vertexArray[m_vertexIndex + 5].position.x = right;
			m_vertexArray[m_vertexIndex + 5].position.z = centerY;
			pAdjNode = FindNode( posSize.x + posSize.size, posSize.y );
			if( pAdjNode != NULL )
			{
				if( pAdjNode->m_bTouched )
				{
					m_vertexArray[m_vertexIndex + 5].position.y = m_elevArray[posSize.y][posSize.x + sizeDivide2];
					m_vertexArray[m_vertexIndex + 5].normal   = m_normalArray[posSize.y][posSize.x + sizeDivide2];
				}
				else
				{
					//m_vertexArray[m_vertexIndex + 5].position.y = m_elevArray[posSize.y][posSize.x + sizeDivide2];
					m_vertexArray[m_vertexIndex + 5].position.y = ( m_elevArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2]
						+ m_elevArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2])/2;
					m_vertexArray[m_vertexIndex + 5].normal = ( m_normalArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2]
						+ m_normalArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2])/2;
				}
			}
			else
			{
				m_vertexArray[m_vertexIndex + 5].position.y = m_elevArray[posSize.y][posSize.x + sizeDivide2];
				m_vertexArray[m_vertexIndex + 5].normal = m_normalArray[posSize.y][posSize.x + sizeDivide2];
			}
			
			// for debug
			//m_vertexArray[m_vertexIndex + 5].position.y = 0;
			m_vertexArray[m_vertexIndex + 5].tu = m_tuArray[posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 5].tv = m_tvArray[posSize.y];
			// 左下角顶点
			m_vertexArray[m_vertexIndex + 6].position.x = left;
			m_vertexArray[m_vertexIndex + 6].position.z = bottom;
			m_vertexArray[m_vertexIndex + 6].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 6].normal   = m_normalArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2];
			// for debug
			//m_vertexArray[m_vertexIndex + 6].position.y = 0;
			m_vertexArray[m_vertexIndex + 6].tu = m_tuArray[posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 6].tv = m_tvArray[posSize.y + sizeDivide2];

			// 下边顶点
			// 判断它左方的那个相邻节点的 m_bTouched 属性是否为true,如为真从数组中读取高程值，否则取相邻顶点高程值的平均值
			m_vertexArray[m_vertexIndex + 7].position.x = centerX;
			m_vertexArray[m_vertexIndex + 7].position.z = bottom;
			pAdjNode = FindNode( posSize.x, posSize.y + posSize.size );
			if( pAdjNode != NULL )
			{
				if( pAdjNode->m_bTouched )
				{
					m_vertexArray[m_vertexIndex + 7].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x];
					m_vertexArray[m_vertexIndex + 7].normal   = m_normalArray[posSize.y + sizeDivide2][posSize.x];
				}
				else
				{
					//m_vertexArray[m_vertexIndex + 7].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x];
					m_vertexArray[m_vertexIndex + 7].position.y = ( m_elevArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2]
						+ m_elevArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2])/2;
					m_vertexArray[m_vertexIndex + 7].normal = ( m_normalArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2]
						+ m_normalArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2])/2;
				}
			}
			else
			{
				m_vertexArray[m_vertexIndex + 7].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x];
				m_vertexArray[m_vertexIndex + 7].normal   = m_normalArray[posSize.y + sizeDivide2][posSize.x];
			}
			
			// for debug
			//m_vertexArray[m_vertexIndex + 7].position.y = 0;
			m_vertexArray[m_vertexIndex + 7].tu = m_tuArray[posSize.x];
			m_vertexArray[m_vertexIndex + 7].tv = m_tvArray[posSize.y + sizeDivide2];
			// 右下角顶点
			m_vertexArray[m_vertexIndex + 8].position.x = right;
			m_vertexArray[m_vertexIndex + 8].position.z = bottom;
			m_vertexArray[m_vertexIndex + 8].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 8].normal   = m_normalArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2];
			// for debug
			//m_vertexArray[m_vertexIndex + 8].position.y = 0;
			m_vertexArray[m_vertexIndex + 8].tu = m_tuArray[posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 8].tv = m_tvArray[posSize.y + sizeDivide2];
			

			//// 顶点数组赋值，   0 1 2
			////                  3 4 5 
			////                  6 7 8
			//// 计算顶点的法线向量
			///*NODE posSize = pNode->GetPosSize();
			//UINT sizeDivide2 = posSize.size >> 1;*/   // 该节点的位置信息（相对）
			//POINT pos[9];  //pos[9]保存了0~8个节点的相对位置信息
			//// pos数组赋值
			//pos[0].x = posSize.x - sizeDivide2;  pos[0].y = posSize.y - sizeDivide2;
			//pos[1].x = posSize.x;                pos[1].y = posSize.y - sizeDivide2;
			//pos[2].x = posSize.x + sizeDivide2;  pos[2].y = posSize.y - sizeDivide2;
			//pos[3].x = posSize.x - sizeDivide2;  pos[3].y = posSize.y;
			//pos[4].x = posSize.x;                pos[4].y = posSize.y;
			//pos[5].x = posSize.x + sizeDivide2;  pos[5].y = posSize.y;
			//pos[6].x = posSize.x - sizeDivide2;  pos[6].y = posSize.y + sizeDivide2;
			//pos[7].x = posSize.x;                pos[7].y = posSize.y + sizeDivide2;
			//pos[8].x = posSize.x + sizeDivide2;  pos[8].y = posSize.y + sizeDivide2;
			//for(int i=0;i<9;i++)
			//{
			//	D3DXVECTOR3 vec[2];
			//	vec[0].x = -m_GridWidth;
			//	// 防止pos[i].x + 1冲出边界
			//	if((UINT)(pos[i].x) < m_width - 1)
			//		vec[0].y = m_elevArray[pos[i].y][pos[i].x] - m_elevArray[pos[i].y][pos[i].x + 1];
			//	else
			//		vec[0].y = m_elevArray[pos[i].y][pos[i].x - 1] - m_elevArray[pos[i].y][pos[i].x];
			//	vec[0].z = 0;

			//	vec[1].x = 0;
			//	// 防止pos[i].y + 1冲出边界
			//	if((UINT)(pos[i].y) < m_width - 1)
			//		vec[1].y = m_elevArray[pos[i].y + 1][pos[i].x] - m_elevArray[pos[i].y][pos[i].x];
			//	else
			//		vec[1].y = m_elevArray[pos[i].y][pos[i].x] - m_elevArray[pos[i].y - 1][pos[i].x];
			//	vec[1].z = m_GridHeight;
			//	D3DXVec3Cross( &m_vertexArray[m_vertexIndex + i].normal, &vec[0], &vec[1] );
			//}


			for(UINT i=0;i<9;i++)
			{
				m_vertexArray[m_vertexIndex+i].color = 0xffffffff;
			}
			// 索引数组赋值，   0 1 2   ，顺时针旋转
			//                  3 4 5 
			//                  6 7 8
			m_indexArray[m_indexIndex + 0]  = m_vertexIndex + 0;
			m_indexArray[m_indexIndex + 1]  = m_vertexIndex + 1;
			m_indexArray[m_indexIndex + 2]  = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 3]  = m_vertexIndex + 1;
			m_indexArray[m_indexIndex + 4]  = m_vertexIndex + 2;
			m_indexArray[m_indexIndex + 5]  = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 6]  = m_vertexIndex + 2;
			m_indexArray[m_indexIndex + 7]  = m_vertexIndex + 5;
			m_indexArray[m_indexIndex + 8]  = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 9]  = m_vertexIndex + 5;
			m_indexArray[m_indexIndex + 10] = m_vertexIndex + 8;
			m_indexArray[m_indexIndex + 11] = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 12] = m_vertexIndex + 8;
			m_indexArray[m_indexIndex + 13] = m_vertexIndex + 7;
			m_indexArray[m_indexIndex + 14] = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 15] = m_vertexIndex + 7;
			m_indexArray[m_indexIndex + 16] = m_vertexIndex + 6;
			m_indexArray[m_indexIndex + 17] = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 18] = m_vertexIndex + 6;
			m_indexArray[m_indexIndex + 19] = m_vertexIndex + 3;
			m_indexArray[m_indexIndex + 20] = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 21] = m_vertexIndex + 3;
			m_indexArray[m_indexIndex + 22] = m_vertexIndex + 0;
			m_indexArray[m_indexIndex + 23] = m_vertexIndex + 4;

			m_vertexIndex += 9;
			m_indexIndex  += 24;
		}
		else
		{
			RefineNode( pNode->m_pTLChildNode );
			RefineNode( pNode->m_pBLChildNode );
			RefineNode( pNode->m_pTRChildNode );
			RefineNode( pNode->m_pBRChildNode );
		}
	}
}


UINT log2(UINT num)
{
	UINT a = 0;
	while(num != 0)
	{
		num = num >> 1;
		a++;
	}
	return a - 1;
}

