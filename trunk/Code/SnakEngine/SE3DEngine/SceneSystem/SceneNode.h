#ifndef _SceneNode_H_
#define _SceneNode_H_
#include <SE3DEngine/Comm/Def.h>
#include <SEMath/Matrix4x4.h>
#include <SE3DEngine/SceneSystem/Entity.h>
#include <SEMath/Vector4.h>
#include <vector>
#include <SEMath/BoundingGeo.h>


namespace SE 
{

    class  SceneNode  
    {
	public:
		enum SOAttrib
		{
			SOA_Cullable = 1UL << 0,
			SOA_Overlay = 1UL << 1,
			SOA_Moveable = 1UL << 2,
			SOA_Unvisible = 1UL << 3
		};
		std::vector<EntityPtr>  ObjectMap;
		SceneNode();
		SceneNode(const String& name);
		~SceneNode();
	protected:
		String m_Name;//id
		SceneNode* m_pParent;
		uint32 m_Attrib;
		//子节点
		bool m_movable;
		Entity* m_pEntityList;
		Matrix4x4 m_NodeMatrix;//node all obj 变换矩阵
		Vector4 m_Position;//节点总体位置
		float m_Scale;//缩放
		AABB aabb;
		
		//边界检查 等

	public:
		void setName(String name);
		String getName();
		//node 绑定entity
        void attachObject(Entity  & obj);
		Entity& detachObject(Entity  & obj);
		void detachAllObjects();
		uint32 GetAttrib();
		//node 树
		SceneNode& createChild();
		SceneNode& removeChild(SceneNode* child);
		void removeAllChild();
		SceneNode addChild(SceneNode* Child);

		void SetParent(SceneNode* parent);
		SceneNode* getParent();


		//node 空间变换
		void setPosition();
		void translate();
        void scale();
		void roll();
		void pitch();
		void rotate();
    };
 
}  

#endif
