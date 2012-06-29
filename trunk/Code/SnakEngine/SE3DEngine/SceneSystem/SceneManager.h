#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
#include <SE3DEngine/Comm/Def.h>
#include <vector>
#include <SE3DEngine/SceneSystem/SceneNode.h>
#include <SE3DEngine/SceneSystem/Frustum.h>
#include <boost/pool/pool_alloc.hpp>

namespace SE
{
	class   SceneManager 
	{
	public:
		typedef std::vector<LightPtr> LightList;
		typedef std::vector<CameraPtr> CameraList;
		typedef std::vector<EntityPtr> EntityList;
		 
		//typedef vector<SceneNodePtr>::type SceneNodeList;

		SceneManager();
		SceneManager(const String& name);
		virtual ~SceneManager();

        virtual void AddLight(LightPtr const& cpLight);
		virtual void DelLight(LightPtr const& cpLight);
	    bool AABBVisible(AABB const &box);
		void AddEntity(EntityPtr const &entity);
		void DelEntity(EntityPtr const &entity);
		void Clear();
	    void ClipScene();
		//virtual void AddNode(SceneNodePtr const &node);
		//virtual void DelNode(SceneNodePtr const &node);

		size_t NumObjectsRendered() const;
		size_t NumRenderablesRendered() const;
		size_t NumPrimitivesRendered() const;
		size_t NumVerticesRendered() const;
	protected:
		 String      m_sName;
		 Color m_vAmbientLight;//光照 
		 LightList      m_pLightList;//场景包含对象
		 Camera*     m_pCamera;
		 Frustum* m_pFrustum; 
		// RenderQueue* m_pRenderQueue;
		 EntityList m_EntityList;//场景obj
		 SceneNode* m_pRootNode;//root 节点
	     //Sky* m_pSky;
		 //Terrain*    m_pTerain;
         //SceneNodeList m_NodeList;
 
		 std::vector<AABBPtr> m_Scene_BbTree;//bb樹
		 std::vector<char> m_VisMarkList;//可見標記lis
         

		 //統計場景各種參數
		size_t numObjectsRendered_;
		size_t numRenderablesRendered_;
		size_t numPrimitivesRendered_;
		size_t numVerticesRendered_;
	};


}
#endif
