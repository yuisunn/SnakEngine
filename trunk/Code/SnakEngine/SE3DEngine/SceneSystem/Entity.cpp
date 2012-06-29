


#include <SE3DEngine/SceneSystem/Entity.h>

namespace SE
{
	Entity::Entity(void)
		: m_vPosition(10.0f),
		m_fScale(1.0f),
		m_fMovEPT(0.0f),
		m_bMovable(false),
		m_bVisible(false),
		m_bTransparent(false),
		m_fRenderDistance(0.0f),
		m_nRenderQueueGroup(10)
	{
	}
	Entity::Entity(String const& name)
		: m_vPosition(10.0f),
		m_fScale(1.0f),
		m_fMovEPT(0.0f),
		m_bMovable(false),
		m_bVisible(false),
		m_bTransparent(false),
		m_fRenderDistance(0.0f),
		m_nRenderQueueGroup(10)
	{
	m_sName = name;
	}


	Entity::~Entity(void)
	{
	}


	Matrix4x4&    Entity::GetEntityMatrix() 
	{
		return m_EntityMatrix;
	}
	Vector4& Entity::GetPosition() 
	{
		return m_vPosition;
	}
	AABB   & Entity::GetBound() 
	{
		return m_AABB;
	}
	uint32 Entity::GetAttrib()
	{
		return m_nAttrib;
	}

	bool Entity::GetVisible() const
	{
		return (0 ==(m_nAttrib & SOA_Unvisible));
	}
	void Entity::SetVisible(bool vis)
	{
		if(vis)
		{
			//~·ûºÅÎ»Çó·´
			m_nAttrib &= ~SOA_Unvisible;
		}
		else
		{
			m_nAttrib |= SOA_Unvisible;
		}
	}
	bool Entity::GetTransparent()
	{
		return m_bTransparent;
	}
	    
}

