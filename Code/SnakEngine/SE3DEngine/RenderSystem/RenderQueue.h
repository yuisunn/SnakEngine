//    ��Ⱦ��������ͨ���ĵط� �ɼ��Լ�� 
//���^�Ќ��F����ͬƽ̨�ă���
//���䲻ͬ��group ���¸��������Ż�
//////////////////////////////////////
#ifndef _RENDERQUEUE_H
#define _RENDERQUEUE_H


#include <SE3DEngine/comm/Def.h>
#include <SE3DEngine/RenderSystem/RenderQueueGroup.h>
#include <SE3DEngine/SceneSystem/Entity.h>
#include <SE3DEngine/SceneSystem/SceneManager.h>

namespace SE
{

    enum ERenderQueueGroupID
    {
		RQ_SKY=0,
		RQ_TERRAIN = 1,
		RQ_ENTITY = 2,
		RQ_TRANSPARENT= 3
    };


class   RenderQueue 
{
	friend class Entity;
	friend class SceneManager;
public:
	typedef std::vector<RenderQueueGroupPtr> RenderQueueGroupList;

protected:
    RenderQueueGroupList* m_pGroups;
	SceneManager* m_pSceneParent;
	

public:
	RenderQueue();
    virtual ~RenderQueue();

	void AddRenderObject(Entity& entity);
	void DelRenderObject();

	void ClipGraphTree();

	//void clear(bool destroyPassMaps = false);
	////�õ���Ⱦgueuegroup
	//RenderQueueGroup* getQueueGroup(uint8 qid);
	////��� ��Ⱦ�����ֵ� queue
	//void addRenderable(Entity* pRend, uint8 groupID, ushort priority);
	//void addRenderable(Entity* pRend, uint8 groupId);
 //   void addRenderable(Entity* pRend);
 //       
 //   //�������Ĭ����Ⱦ����
	//void setDefaultQueueGroup(uint8 grp);
 //   uint8 getDefaultQueueGroup(void) const;
 //   //����Ĭ�����ȼ�
 //   void setDefaultRenderablePriority(ushort priority);
	////������Ӱpass
 //   //�ϲ�renderqueue
	//void merge( const RenderQueue* rhs );
    // �ɼ�������ӵ� ����
   };
}
#endif