#ifndef _D3D11GraphBuffer_H_
#define _D3D11GraphBuffer_H_

#include <SE3DEngine/RenderSystem/CGraphBuffer.h>
#include <SE3DEngine/Comm/PixelFormat.h>
#include <SE3DEngine/Comm/Def.h>
#include <d3d11.h>

namespace SE
{
	class _DLL_API D3D11GraphBuffer : public CGraphBuffer
	{
	public:
		explicit D3D11GraphBuffer(EBufferUsage eUsage, uint32 uAccess, uint32 uBindFlag, PixelInitData const* cpInitData, PixelFormat Fmt);
		~D3D11GraphBuffer();
		void CopyToBuffer(CGraphBuffer& pRhs);

		ID3D11BufferPtr const & GetD3DBuffer() const
		{
			return m_pBuf;
		}

		ID3D11ShaderResourceViewPtr const & GetD3DShaderResourceView() const
		{
			return m_pShaderRes;
		}

		ID3D11UnorderedAccessViewPtr const & GetD3DUnorderedAccessView() const
		{
			return m_pUnorderAcces;
		}
		void GetD3DFlags(D3D11_USAGE& usage, UINT& cpu_access_flags, UINT& bind_flags, UINT& misc_flags);

	    void DoResize();
	    void* Map(EBufferAccess eAcces);
		void Unmap();
	private:
		ID3D11DevicePtr m_pDevice;
		ID3D11DeviceContextPtr m_pImmContext;
		ID3D11BufferPtr m_pBuf;
		ID3D11ShaderResourceViewPtr m_pShaderRes;
		ID3D11UnorderedAccessViewPtr m_pUnorderAcces;

		uint32 m_uBindFlag;
		uint32 m_uBufSize;
		PixelFormat m_FmtShader;


	};
	typedef boost::shared_ptr<D3D11GraphBuffer> D3D11GraphBufferPtr;
}

#endif
