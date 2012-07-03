////可移动对象
///////////////////////////
//#ifndef _Entity_H_
//#define _Entity_H_
//
//#include <SE3DEngine/Comm/Def.h>
//#include <SE3DEngine/SceneSystem/SceneNode.h>
//#include <SE3DEngine/RenderSystem/RenderQueueGroup.h>
//#include <SE3DEngine/Mesh/RenderMesh.h>
//#include <SEMath/Matrix4x4.h>
//#include <SEMath/Vector4.h>
//
//namespace SE
//{ 
//	 
//	//可移动对象
//	//
//	class  Entity //继承renderable？？？？
//	{
//		
//	public:
//		//对象要检测的类型
//		enum ESOAttrib
//		{
//			SOA_Cullable = 1UL << 0,
//			SOA_Overlay = 1UL << 1,
//			SOA_Moveable = 1UL << 2,
//			SOA_Unvisible = 1UL << 3
//		};
//		Entity();
//		Entity(String const& name);
//		~Entity();
//
//		virtual Matrix4x4& GetEntityMatrix() ;
//		virtual Vector4& GetPosition() ;
//		virtual AABB  & GetBound() ;
//		virtual uint32 GetAttrib();
//		virtual bool GetVisible() const;
//		virtual void SetVisible(bool vis);
//		virtual bool GetTransparent();
//		//空间变换
//		void setPosition();
//		void translate();
//        void scale();
//		void roll();
//		void pitch();
//		void rotate();
//		//碰撞检测
//	    
//	protected:
//		String m_sName;
//		Matrix4x4 m_EntityMatrix;//node all obj 变换矩阵
//		Vector4 m_vPosition;//节点总体位置
//		float m_fScale;//缩放
//		SceneNode* m_pParent;//在那个node attach
//		float m_fMovEPT;
//		bool m_bMovable;
//		bool m_bVisible;
//		bool m_bTransparent;
//		float m_fRenderDistance;
//		uint8 m_nRenderQueueGroup;
//		AABB m_AABB;
//		IRenderMesh* m_pMesh;
//		uint32 m_nAttrib;
//		ERenderQueueGroupID m_eRenderGroupID;
//
//	};
//
//
//}
//
//#endif