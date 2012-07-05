//�൥��ͷ��ĸ��д
//����Ҳ��
//�ṹ ͷ�Ӵ�S
//ö�� ͷ��daE
//��Ա���� m_
//��������ʱ������_
//������ʱ�����ڵ����м�_ �����ط��ı�������
//typdef �����Ҫ��ͷ��
//�����м��� ��������
//float f int n unsigned �� long ������� n ��ʾ  Vector v Matrix m char c
//ָ���p ���ü�r
//����stl ������t

//���� ��ͷ�� C��i ��ʾ����

//include ˳�� Ӧʹ�������˳��C��׼�⡢C++��׼�⡢�������ͷ�ļ������Լ����̵�ͷ�ļ�


#ifndef _DEF_H_
#define _DEF_H_

#include <String>
#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <boost/smart_ptr.hpp>

#include <d3d11.h>

#include <SEMath/Vector4.h>
#include <SE3DEngine/Config/Platform.h>



#pragma warning(disable:4251)



//�궨��

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

//c++�﷨����
#define ILINE _inline

//��ƽ̨ dll
#ifdef SE_WINDOWS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API  
#endif


//����ָ��
namespace SE
{
#define HashMap ::std::tr1::unordered_map

 //ǰ������
		class  ConditionalRender
	{
	public:
		virtual ~ConditionalRender()
		{
		}

		virtual void BeginConditionalRender() = 0;
		virtual void EndConditionalRender() = 0;

		virtual bool AnySamplesPassed() = 0;
	};
		typedef boost::scoped_ptr<ConditionalRender> ConditionalRenderPtr;
		//ƽ̨���
		class AppFramework;
		typedef boost::shared_ptr<AppFramework> AppFrameworkPtr;
		class Window;
		typedef boost::shared_ptr<Window> WindowPtr;
 //��Ƶ
	class CAudio;
	typedef boost::shared_ptr<CAudio> CAudioPtr;
	class CAudioBuffer;
	typedef boost::shared_ptr<CAudioBuffer> CAudioBufferPtr;
    class CAudioDataSource;
    typedef boost::shared_ptr<CAudioDataSource> CAudioDataSourcePtr;
    class CAudioDataSourceFactory;
    typedef boost::shared_ptr<CAudioDataSourceFactory> CAudioDataSourceFactoryPtr;
    class CAudioFactory;
    typedef boost::shared_ptr<CAudioFactory> CAudioFactoryPtr;
    class ConcreteAudioFactory;
    typedef boost::shared_ptr<ConcreteAudioFactory> ConcreteAudioFactoryPtr;


 class RenderLayout;
 //d3d ���� ΢��
 typedef boost::shared_ptr<ID3D11Device> ID3D11DevicePtr;
 typedef boost::shared_ptr<ID3D11DeviceContext> ID3D11DeviceContextPtr;
 typedef boost::shared_ptr<ID3D11Buffer> ID3D11BufferPtr;
 typedef boost::shared_ptr<ID3D11ShaderResourceView> ID3D11ShaderResourceViewPtr;
 typedef boost::shared_ptr<ID3D11UnorderedAccessView> ID3D11UnorderedAccessViewPtr;
typedef boost::shared_ptr<IDXGIFactory>					IDXGIFactoryPtr;
typedef boost::shared_ptr<IDXGIAdapter>					IDXGIAdapterPtr;
typedef boost::shared_ptr<IDXGIFactory1>				IDXGIFactory1Ptr;
typedef boost::shared_ptr<IDXGIAdapter1>				IDXGIAdapter1Ptr;
typedef boost::shared_ptr<IDXGISwapChain>				IDXGISwapChainPtr;

typedef boost::shared_ptr<ID3D11Resource>				ID3D11ResourcePtr;
typedef boost::shared_ptr<ID3D11Texture1D>				ID3D11Texture1DPtr;
typedef boost::shared_ptr<ID3D11Texture2D>				ID3D11Texture2DPtr;
typedef boost::shared_ptr<ID3D11Texture3D>				ID3D11Texture3DPtr;
typedef boost::shared_ptr<ID3D11Texture2D>				ID3D11TextureCubePtr;
typedef boost::shared_ptr<ID3D11InputLayout>			ID3D11InputLayoutPtr;
typedef boost::shared_ptr<ID3D11Query>					ID3D11QueryPtr;
typedef boost::shared_ptr<ID3D11Predicate>				ID3D11PredicatePtr;
typedef boost::shared_ptr<ID3D11VertexShader>			ID3D11VertexShaderPtr;
typedef boost::shared_ptr<ID3D11PixelShader>			ID3D11PixelShaderPtr;
typedef boost::shared_ptr<ID3D11GeometryShader>			ID3D11GeometryShaderPtr;
typedef boost::shared_ptr<ID3D11ComputeShader>			ID3D11ComputeShaderPtr;
typedef boost::shared_ptr<ID3D11HullShader>				ID3D11HullShaderPtr;
typedef boost::shared_ptr<ID3D11DomainShader>			ID3D11DomainShaderPtr;
typedef boost::shared_ptr<ID3D11RenderTargetView>		ID3D11RenderTargetViewPtr;
typedef boost::shared_ptr<ID3D11DepthStencilView>		ID3D11DepthStencilViewPtr;

typedef boost::shared_ptr<ID3D11RasterizerState>		ID3D11RasterizerStatePtr;
typedef boost::shared_ptr<ID3D11DepthStencilState>		ID3D11DepthStencilStatePtr;
typedef boost::shared_ptr<ID3D11BlendState>				ID3D11BlendStatePtr;
typedef boost::shared_ptr<ID3D11SamplerState>			ID3D11SamplerStatePtr;

//typedef boost::shared_ptr<ID3DBlob>						ID3DBlobPtr;

class Entity;
typedef boost::shared_ptr<Entity> EntityPtr;
class SceneNode;
typedef boost::shared_ptr<SceneNode> SceneNodePtr;
class Camera;
typedef boost::shared_ptr<Camera> CameraPtr;
class CLightSource;
typedef boost::shared_ptr<CLightSource> LightPtr;
struct AABB;
typedef boost::shared_ptr<AABB> AABBPtr;

class CTexture;
typedef boost::shared_ptr<CTexture> CTexturePtr;

struct SRenderMaterial;
typedef boost::shared_ptr<SRenderMaterial> SRenderMaterialPtr;
class RenderQueueGroup;
typedef boost::shared_ptr<RenderQueueGroup> RenderQueueGroupPtr;
class CGraphBuffer;
typedef boost::shared_ptr<CGraphBuffer> CGraphBufferPtr;
struct SShaderParam;
typedef boost::shared_ptr<SShaderParam>  SShaderParamPtr;
struct SShaderTechnique;
typedef boost::shared_ptr<SShaderTechnique> SShaderTechniquePtr;
struct SShaderPass;
typedef boost::shared_ptr<SShaderPass> SShaderPassPtr;
struct SShaderHandle;
typedef boost::shared_ptr<SShaderHandle>  SShaderHandlePtr;
struct IShader;
typedef boost::shared_ptr<IShader> IShaderPtr;
class CRender;
typedef boost::shared_ptr<CRender> CRenderPtr;
class RenderView;
typedef boost::shared_ptr<RenderView> RenderViewPtr;
class CFrameBuffer;
typedef boost::shared_ptr<CFrameBuffer> CFrameBufferPtr;

struct STexMaterial;
typedef boost::shared_ptr<STexMaterial> STexMaterialPtr;
struct IMaterial;
typedef boost::shared_ptr<IMaterial>  IMaterialPtr;
class RenderTarget;
typedef boost::shared_ptr<RenderTarget>  RenderTargetPtr;
struct IRneder;
typedef boost::shared_ptr<IRneder> IRenderPtr;
struct RenderPipeline;
typedef boost::shared_ptr<RenderPipeline> RenderPipelinePtr;

//����
class SceneManager;
typedef boost::shared_ptr<SceneManager> SceneMgrPtr;
class CAPIObjFactory;
typedef boost::shared_ptr<CAPIObjFactory> CAPIObjFactoryPtr;
class CRender;
typedef boost::shared_ptr<CRender> CRenderPtr;


class XmlDoc;
typedef boost::shared_ptr<XmlDoc> XmlDocPtr;
class XmlNode;
typedef boost::shared_ptr<XmlNode> XmlNodePtr;
class XmlAttribute;
typedef boost::shared_ptr<XmlAttribute> XmlAttributePtr;

//��Դ��ȡ 
class IResource;
typedef boost::shared_ptr<IResource> IResourcePtr;
class CFileDataStream;
typedef boost::shared_ptr<CFileDataStream> CFileDataStreamPtr;
class DataStreamXml;
typedef boost::shared_ptr<DataStreamXml> DataStreamXmlPtr;
class SEResourceManager;
typedef boost::shared_ptr<SEResourceManager> SEResourceManagerPtr;
//��Դ�洢�ṹ
class ModelDesc;
typedef boost::shared_ptr<ModelDesc> ModelDescPtr;
//��Ⱦ״̬
class CRasterizerStateObj;
typedef boost::shared_ptr<CRasterizerStateObj> CRasterizerStateObjPtr;
class CDepthStencilStateObj;
typedef boost::shared_ptr<CDepthStencilStateObj> CDepthStencilStateObjPtr;
class CBlendStateObj;
typedef boost::shared_ptr<CBlendStateObj> CBlendStateObjPtr;
class CSamplerStateObj;
typedef boost::shared_ptr<CSamplerStateObj> CSamplerStateObjPtr;

//��������

typedef std::string String;
typedef std::wstring WString;
typedef Vector4 Color;
typedef unsigned long long int ResourceHandle;

//��������������չ����
typedef unsigned char byte;
typedef unsigned int size_t;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef uint64_t uint64;
typedef int64_t int64;

typedef int int32;
typedef short int16;
typedef char int8;
typedef float float32;

//typedef unsigned char uchar;
//typedef unsigned short ushort;
//typedef unsigned int uint;
//typedef unsigned long ulong;
//
//typedef unsigned short ushort;
//typedef unsigned int uint;
//��ɫ
}

#endif
