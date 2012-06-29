#include <SE3DEngine/RenderSystem/RenderQueue.h>
#include <SE3DEngine/Comm/Def.h>

namespace SE{

	// ≥ı ºªØ
	////////////////////////////
	RenderQueue::RenderQueue()       
	{
	}
	//œ°ππ
	//////////////
	RenderQueue::~RenderQueue()
    {
    }

	void RenderQueue::AddRenderObject(Entity& entity)
	{
		 switch(entity.m_eRenderGroupID)
		 {
		 case RQ_ENTITY:
			 m_pGroups[RQ_ENTITY].AddRenderObject(entity) ;
			  break;
		 case RQ_SKY:
			 m_pGroups[RQ_SKY].AddRenderObject(entity) ;
			 break;
		 case RQ_TERRAIN:
			 m_pGroups[RQ_TERRAIN].AddRenderObject(entity) ;
			 break;
		 case RQ_TRANSPARENT:
			 m_pGroups[RQ_TRANSPARENT].AddRenderObject(entity) ;
			 break;
		 default:
			 break;
		 }

	}
	void RenderQueue::DelRenderObject()
	{

	}
	void RenderQueue::ClipGraphTree()
	{
		for(int i=0; m_pSceneParent.m_VisMarkList.size(); i++)
		{
			if(m_VisMarkList[i])
			{
				AddRenderObject(m_pSceneParent.m_EntityList[i]);
			}
		}
	}



}
