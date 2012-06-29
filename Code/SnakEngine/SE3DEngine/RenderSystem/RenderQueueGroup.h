//只負責渲染對象分組 和排序
#ifndef _RenderQueueGroup_H_
#define _RenderQueueGroup_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/RenderSystem/RenderQueue.h>
#include <vector>
#include <map>

namespace SE
{
	//选然对象根据那些枚举的几个类型分组 后由下个独享优化
	class   RenderQueueGroup 
	{
		friend class Entity;
    public:
		typedef std::vector<EntityPtr> PriorityList;
		

	   
		RenderQueueGroup();
		~RenderQueueGroup();
		void AddRenderObject(Entity& entity);
	    void DelRenderObject();
		void DistanceSort();
	protected:
		RenderQueue* m_Parent;
		ERenderQueueGroupID m_GroupID;
		PriorityList* m_pPriority;

	   /* bool m_SplitNoShCrackadowPasses;
		bool m_ShadowCastersNotReceivers;*/
		//判断优先级那个玩意的obj
		//阴影是否是用queue

		
	};
}

#endif
