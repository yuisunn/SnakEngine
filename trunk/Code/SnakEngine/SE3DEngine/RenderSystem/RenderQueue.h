//    渲染对象最先通过的地方 可见性检测 
//可繼承實現對不同平台的優化
//分配不同的group 后下个对象在优化
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
	////得到渲染gueuegroup
	//RenderQueueGroup* getQueueGroup(uint8 qid);
	////添加 渲染对象现到 queue
	//void addRenderable(Entity* pRend, uint8 groupID, ushort priority);
	//void addRenderable(Entity* pRend, uint8 groupId);
 //   void addRenderable(Entity* pRend);
 //       
 //   //获得设置默认渲染队列
	//void setDefaultQueueGroup(uint8 grp);
 //   uint8 getDefaultQueueGroup(void) const;
 //   //设置默认优先级
 //   void setDefaultRenderablePriority(ushort priority);
	////不用阴影pass
 //   //合并renderqueue
	//void merge( const RenderQueue* rhs );
    // 可见对象添加到 队列
   };
}
#endif