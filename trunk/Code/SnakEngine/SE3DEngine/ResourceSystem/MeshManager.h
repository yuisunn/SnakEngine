#ifndef _MeshManager_H_
#define _MeshManager_H_

#include <SE3DEngine/ResourceSystem/IResource.h>
#include <SE3DEngine/Comm/Def.h>
#include <memory>
#include <map>
#include <SE3DEngine/ResourceSystem/IResManager.h>

namespace SE
{
	class MeshManager : public IResManager
	{
	public:                  
		static MeshManager* Instance();
		virtual void AddPath(String const & path);
		virtual IResourcePtr Create(String const &sName, bool bBackgroundThread=false);
		virtual IResourcePtr Prepare(String const &sName,bool bBackgroundThread=false);
		virtual IResourcePtr Load(String const &sName,bool bBackgroundThread=false);
		virtual void Reload(String const &sName);
		virtual void Unload(String const &sName);
		virtual void UnloadAll();
		virtual void ReloadALL();
		virtual bool ResExists(String const &sName);


		~MeshManager();
	protected:
		MeshManager();
		static MeshManager* m_pInstance;
		HashMap<String, ModelDescPtr> m_meshMaps
		 
	};
}
#endif

