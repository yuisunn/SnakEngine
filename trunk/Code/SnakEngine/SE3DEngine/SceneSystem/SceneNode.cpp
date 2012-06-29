#include <SE3DEngine/SceneSystem/Scenenode.h>

namespace SE
{
	SceneNode::SceneNode(const String& name)
	  : m_movable(false),
		m_Position(10.0f),
		m_Scale(1.0f)
	{
		m_Name = name;
	}

		void SceneNode::setName(String name)
		{
			m_Name = name;
		}
		String SceneNode::getName()
		{
			return m_Name;
		}
		
        void SceneNode::attachObject(Entity   & obj)
		{

		}
		Entity& SceneNode::detachObject(Entity & obj)
		{
			return obj;

		}
		uint32 SceneNode::GetAttrib() 
		{
			return m_Attrib;
		}
		void SceneNode::detachAllObjects()
		{

		}
		//node Ê÷
		//SceneNode* SceneNode::createChild();
		//SceneNode* SceneNode::removeChild(SceneNode* child);
		//void SceneNode::removeAllChild();
		//SceneNode* SceneNode::addChild(SceneNode* Child);

		//void SceneNode::SetParent(SceneNode* parent)
		//SceneNode* SceneNode::getParent();
}