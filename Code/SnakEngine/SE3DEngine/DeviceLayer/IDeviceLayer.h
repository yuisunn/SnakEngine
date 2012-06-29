//对不同api 定义统一接口类
#ifndef _Device_H_
#define _Device_H_

#include <SE3DEngine/Config/Platform.h>
#include <d3d9.h>

namespace SE
{
	//一般来讲，当一个类中有必要进行数据隐藏时，请用class声明，
	//并将私有数据标记为private，公共接口标记为public；而当所有成员都有必要成为公有成员的时候，
		//请用struct来声明它。 全都是 public
 
	#if defined (DIRECT3D9)
	 
		//dx9 设备定义
		static LPDIRECT3DDEVICE9 m_pd3dDevice;
		LPDIRECT3D9 m_pD3D;
		D3DPRESENT_PARAMETERS m_pD3DPresent;         // Parameters for CreateDevice/Reset
	    D3DCAPS9   *m_pd3dCaps;           // Caps for the device
		D3DSURFACE_DESC   m_d3dsdBackBuffer;      // Surface desc of the Backbuffer
		D3DFORMAT         m_ZFormat;
	 
		static LPDIRECT3DDEVICE9  GetDevice() { return m_pd3dDevice; }
		LPDIRECT3D9        GetD3D() { return m_pD3D; }
		const D3DCAPS9     *GetD3DCaps()  { return m_pd3dCaps; }

		void Init(HWND hwnd)
		{
			if(NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
				return;
			//填充m_pD3DPresent
			ZeroMemory(&m_pD3DPresent, sizeof(m_pD3DPresent));
			m_pD3DPresent.Windowed = TRUE;
			m_pD3DPresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
			m_pD3DPresent.BackBufferFormat = D3DFMT_UNKNOWN;
			m_pD3DPresent.BackBufferCount = 1;
			m_pD3DPresent.EnableAutoDepthStencil = TRUE;
			m_pD3DPresent.AutoDepthStencilFormat = D3DFMT_D24S8;
			m_pD3DPresent.MultiSampleType = D3DMULTISAMPLE_NONE;
			m_pD3DPresent.hDeviceWindow = hwnd;
			//创建 d3ddevice
			if(D3D_OK != (m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_pD3DPresent, &m_pd3dDevice)))
				return;
		}
		void Close()
		{
		if(m_pd3dDevice)
		{
			m_pd3dDevice->Release();
			m_pd3dDevice = NULL;
		}

		if(m_pD3D)
		{
			m_pD3D->Release();
			m_pD3D = NULL;
		}

	}


	#elif defined (DIRECT3D10)
		D3DDevice*  GetDevice() { return m_pd3dDevice; }
		D3DDevice*	GetD3DDevice() { return m_pd3dDevice; }
		ID3D11DeviceContext*  GetDeviceContext() { return m_pd3dDeviceContext; }
		#endif
}
#endif




