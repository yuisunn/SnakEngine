//ֻؓ؟��Ⱦ����ֽM ������
#ifndef _RenderQueueGroup_H_
#define _RenderQueueGroup_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/RenderSystem/RenderQueue.h>
#include <vector>
#include <map>

namespace SE
{
	//ѡȻ���������Щö�ٵļ������ͷ��� �����¸������Ż�
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
		//�ж����ȼ��Ǹ������obj
		//��Ӱ�Ƿ�����queue

		
	};
}

#endif
