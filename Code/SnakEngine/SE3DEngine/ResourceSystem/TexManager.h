#ifndef _TexManager_H_
#define _Texanager_H_

#include <SE3DEngine/ResourceSystem/IResource.h>
#include <SE3DEngine/Comm/Def.h>
#include <memory>
#include <vector>
#include <map>
#include <SE3DEngine/ResourceSystem/IResManager.h>
#include <SE3DEngine/Config/Platform.h>

namespace SE
{
	class TexManager : public IResManager
	{
	public:                  
		static TexManager* Instance();
		void AddPath(String const & sPath);
		void Create(String const &sName, bool bBackgroundThread=false);
		IResourcePtr Prepare(String const &sName,bool bBackgroundThread=false);
		IResourcePtr Load(String const &sName,bool bBackgroundThread=false);
		void Reload(String const &sName);
		void Unload(String const &sName);
		void UnloadAll();
		void ReloadALL();
		bool ResExists(String const &sName);


		~TexManager();
	protected:
		TexManager();
		static TexManager* m_pInstance;
		String sExePath_;
		//std::vector<String> m_sPathes;
	    HashMap<String, ModelDescPtr> m_texMaps;
		

		 
	};
}
#endif

