#include <D3D11/D3D11GraphBuffer.h>
#include <SE3DEngine/Comm/ThrowErr.h>
#include <boost/assert.hpp>
#include <D3D11/D3D11Render.h>
#include <SE3DEngine/Comm/MonFac.h>
#include <D3D11/D3D11Mapping.h>

namespace SE
{
	D3D11GraphBuffer::D3D11GraphBuffer(EBufferUsage eUsage, uint32 uAccess, uint32 uBindFlag, PixelInitData const* cpInitData, PixelFormat Fmt)
	: CGraphBuffer(eUsage, uAccess),
	  m_uBindFlag(uBindFlag), m_uBufSize(0), m_FmtShader(Fmt)
	{
		if ((m_uAcces & EAH_GPU_Unordered) && (m_FmtShader != EF_Unknown))
		{
			m_uBindFlag = 0;
		}

		D3D11Render const & ren(*checked_cast<D3D11Render const *>(&MonFac::Only().InstFactory().InstRender()));
		m_pDevice = ren.GetD3DDevice();
		m_pImmContext = ren.GetD3DDeviceImmContext();
		m_uByteSize = 0;
		//��д desc �ṹ
		if (cpInitData != NULL)
		{
			D3D11_BUFFER_DESC buf_desc;
			this->GetD3DFlags(buf_desc.Usage, buf_desc.CPUAccessFlags, buf_desc.BindFlags, buf_desc.MiscFlags);
			//���ݳ���
			buf_desc.ByteWidth = cpInitData->m_uRowPitch;
			m_uByteSize = cpInitData->m_uRowPitch;
			//�����ʽ
			buf_desc.StructureByteStride = NumFormatBytes(m_FmtShader);

			//����������
			D3D11_SUBRESOURCE_DATA subres_init;
			subres_init.pSysMem = cpInitData->m_cpData;
			subres_init.SysMemPitch = cpInitData->m_uRowPitch;
			subres_init.SysMemSlicePitch = cpInitData->m_uSlicePitch;
			//���Կ�ʼ���� buffer
			ID3D11Buffer* buffer;
			ErO(m_pDevice->CreateBuffer(&buf_desc, &subres_init, &buffer));
			m_pBuf = MakeCOMPtr(buffer);
			m_uBufSize = this->GetSize();
			//��дbuffer ��ϸ��Ϣ
			if ((m_uAcces & EAH_GPU_Read) && (m_FmtShader != EF_Unknown))
			{
				//shader ��Դ view
				D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
				sr_desc.Format = (m_uAcces & EAH_GPU_Structured) ? DXGI_FORMAT_UNKNOWN : D3D11Mapping::MappingFormat(m_FmtShader);
				//��Դ����
				sr_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				//��ͼ���ʵĵ�һ��Ԫ��
				sr_desc.Buffer.ElementOffset = 0;
				//ȫ����Ա����
				sr_desc.Buffer.ElementWidth = m_uByteSize / NumFormatBytes(m_FmtShader);
				//����buf ����shader��Դ
				ID3D11ShaderResourceView* d3d_sr_view;
				ErO(m_pDevice->CreateShaderResourceView(m_pBuf.get(), &sr_desc, &d3d_sr_view));
				m_pShaderRes = MakeCOMPtr(d3d_sr_view);
			}
			//���������Դ view
			if ((m_uAcces & EAH_GPU_Unordered) && (m_FmtShader != EF_Unknown))
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
				//��Դ��ʽ
			     // DXGI_FORMAT_R8_SNORM  = 63,DXGI_FORMAT_R8_SINT  = 64, DXGI_FORMAT_A8_UNORM ��  
				uav_desc.Format = DXGI_FORMAT_UNKNOWN;
				//��Դ���� ��η���ָ����Դ
				uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uav_desc.Buffer.FirstElement = 0;
				//Ԫ�ظ���
				uav_desc.Buffer.NumElements = this->GetSize() / buf_desc.StructureByteStride;
				//�鿴��Դѡ��
				uav_desc.Buffer.Flags = 0;
				//����������ͼ
				ID3D11UnorderedAccessView* d3d_ua_view;
				ErO(m_pDevice->CreateUnorderedAccessView(m_pBuf.get(), &uav_desc, &d3d_ua_view));
				m_pUnorderAcces = MakeCOMPtr(d3d_ua_view);
			}
		}
	}




	void D3D11GraphBuffer::GetD3DFlags(D3D11_USAGE& peUsage, UINT& puCpuAccessFlag, UINT& puBindFlag, UINT& puMiscFlag)
	{
		if (m_uAcces & EAH_Immutable)
		{
			//�������� ֻ�ܱ�gpu��ȡ  �����Ժ�Ͳ��ܸı�
			peUsage = D3D11_USAGE_IMMUTABLE;
		}
		else
		{
			if ((EAH_CPU_Write == m_uAcces) || ((EAH_CPU_Write | EAH_GPU_Read) == m_uAcces))
			{
				peUsage = D3D11_USAGE_DYNAMIC;
			}
			else
			{
				if (!(m_uAcces & EAH_CPU_Read) && !(m_uAcces & EAH_CPU_Write))
				{
					peUsage = D3D11_USAGE_DEFAULT;
				}
				else
				{
					peUsage = D3D11_USAGE_STAGING;
				}
			}
		}

		puCpuAccessFlag = 0;
		if (m_uAcces & EAH_CPU_Read)
		{
			//�������
			puCpuAccessFlag |= D3D11_CPU_ACCESS_READ;
		}
		if (m_uAcces & EAH_CPU_Write)
		{
			//��������
			puCpuAccessFlag |= D3D11_CPU_ACCESS_WRITE;
		}

		if (D3D10_USAGE_STAGING == peUsage)
		{
			puBindFlag = 0;
		}
		else
		{
			puBindFlag = m_uBindFlag;
		}
		D3D11Render const & ren = *checked_cast<D3D11Render const *>(&MonFac::Only().InstFactory().InstRender());
		if (ren.DeviceFeatureLevel() > D3D_FEATURE_LEVEL_9_3)
		{
			if (m_uAcces & EAH_GPU_Read)
			{
				m_uBindFlag |= D3D11_BIND_SHADER_RESOURCE;
			}
			if (m_uAcces & EAH_GPU_Write)
			{
				m_uBindFlag |= D3D11_BIND_STREAM_OUTPUT;
			}
			if (m_uAcces & EAH_GPU_Unordered)
			{
				m_uBindFlag |= D3D11_BIND_UNORDERED_ACCESS;
			}
		}

		puMiscFlag = 0;
		if (m_uAcces & EAH_GPU_Unordered)
		{
			puMiscFlag = (m_uAcces & EAH_GPU_Structured)
				? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}
	}

	void D3D11GraphBuffer::DoResize()
	{
		BOOST_ASSERT(m_uByteSize != 0);

		if (this->GetSize() > m_uBufSize)
		{
			D3D11_BUFFER_DESC desc;
			this->GetD3DFlags(desc.Usage, desc.CPUAccessFlags, desc.BindFlags, desc.MiscFlags);
			desc.ByteWidth = m_uByteSize;
			desc.StructureByteStride = NumFormatBytes(m_FmtShader);

			ID3D11Buffer* buffer;
			ErO(m_pDevice->CreateBuffer(&desc, NULL, &buffer));
			m_pBuf = MakeCOMPtr(buffer);
			m_uBufSize = this->GetSize();

			if ((m_uAcces & EAH_GPU_Read) && (m_FmtShader != EF_Unknown))
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
				sr_desc.Format = (m_uAcces & EAH_GPU_Structured) ? DXGI_FORMAT_UNKNOWN : D3D11Mapping::MappingFormat(m_FmtShader);
				sr_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				sr_desc.Buffer.ElementOffset = 0;
				sr_desc.Buffer.ElementWidth = m_uByteSize / NumFormatBytes(m_FmtShader);

				ID3D11ShaderResourceView* d3d_sr_view;
				ErO(m_pDevice->CreateShaderResourceView(m_pBuf.get(), &sr_desc, &d3d_sr_view));
				m_pShaderRes = MakeCOMPtr(d3d_sr_view);
			}

			if ((m_uAcces & EAH_GPU_Unordered) && (m_FmtShader != EF_Unknown))
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
				uav_desc.Format = DXGI_FORMAT_UNKNOWN;
				uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uav_desc.Buffer.FirstElement = 0;
				uav_desc.Buffer.NumElements = this->GetSize() / desc.StructureByteStride;
				uav_desc.Buffer.Flags = 0;

				ID3D11UnorderedAccessView* d3d_ua_view;
				ErO(m_pDevice->CreateUnorderedAccessView(m_pBuf.get(), &uav_desc, &d3d_ua_view));
				m_pUnorderAcces = MakeCOMPtr(d3d_ua_view);
			}
		}
	}
	//����gpu
	void* D3D11GraphBuffer::Map(EBufferAccess eAcces)
	{
		BOOST_ASSERT(m_pBuf);

		D3D11_MAP type;
		switch (eAcces)
		{
		case BA_Read_Only:
			type = D3D11_MAP_READ;
			break;

		case BA_Write_Only:
			if ((EAH_CPU_Write == m_uAcces) || ((EAH_CPU_Write | EAH_GPU_Read) == m_uAcces))
			{
				type = D3D11_MAP_WRITE_DISCARD;
			}
			else
			{
				type = D3D11_MAP_WRITE;
			}
			break;

		case BA_Read_Write:
			type = D3D11_MAP_READ_WRITE;
			break;

		default:
			BOOST_ASSERT(false);
			type = D3D11_MAP_READ;
			break;
		}
		//���� һ��gpu ����ʱ���ܸ��ĵ�ֵ
		//�ڴ�ӳ�� ���뵽gpu �ڴ�
		D3D11_MAPPED_SUBRESOURCE mapped;
		ErO(m_pImmContext->Map(m_pBuf.get(), 0, type, 0, &mapped));
		return mapped.pData;
	}

	void D3D11GraphBuffer::Unmap()
	{
		BOOST_ASSERT(m_pBuf);

		m_pImmContext->Unmap(m_pBuf.get(), 0);
	}

	void D3D11GraphBuffer::CopyToBuffer(CGraphBuffer& pRhs)
	{
		BOOST_ASSERT(this->GetSize() == pRhs.GetSize());

		D3D11GraphBuffer& buf = *checked_cast<D3D11GraphBuffer*>(&pRhs);
		m_pImmContext->CopyResource(buf.GetD3DBuffer().get(), m_pBuf.get());
	}
}
