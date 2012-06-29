#include <SE3DEngine/ResourceSystem/MeshManager.h>

 
namespace SE
{

	MeshManager* MeshManager::Instance()
    {
		if(!m_pInstance)
			m_pInstance = new MeshManager();
		return m_pInstance;

	}
}