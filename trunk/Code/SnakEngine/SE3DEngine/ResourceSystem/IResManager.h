//资源地址检测
//单利

#ifndef _ResManager_H_
#define _ResManager_H_

#include <SE3DEngine/ResourceSystem/IResource.h>
#include <SE3DEngine/Comm/Def.h>


namespace SE
{
	struct _DLL_API IResManager
	{
			 
			virtual void AddPath(String const & path) = 0;
			virtual IResourcePtr Create(String const &sName, bool bBackgroundThread=false) = 0;
			virtual IResourcePtr Prepare(String const &sName,bool bBackgroundThread=false) = 0;
			virtual IResourcePtr Load(String const &sName,bool bBackgroundThread=false) = 0;
			virtual void Reload(String const &sName);
			virtual void Unload(String const &sName);
			virtual void UnloadAll() = 0;
			virtual void ReloadALL() = 0;
			virtual bool ResExists(String const &sName) = 0;
	};


}

#endif