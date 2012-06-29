#include <SE3DEngine/SceneSystem/SceneManager.h>
#include <SE3DEngine/Comm/Def.h>
#include <boost/typeof/typeof.hpp>


namespace SE
{
	SceneManager::SceneManager(const String& sName)
	{
		m_sName = sName;
	}
	//���
	void SceneManager::Clear()
	{
		m_EntityList.reserve(0);
		m_Scene_BbTree.reserve(0);

	}
	//�ɼ����
	bool SceneManager::AABBVisible(AABB const & box)
	{
		return m_pFrustum->VisibleAABB(box) != Frustum::VIS_NO;
	}
	//��ӵƹ�
    void SceneManager::AddLight(LightPtr const& cpLight)
	{
		m_pLightList.push_back(light);
	}
	//ɾ��
    void SceneManager::DelLight(LightPtr const& cpLight)
	{
		BOOST_AUTO(iter, std::find(m_pLightList.begin(), m_pLightList.end(), light));
		m_pLightList.erase(iter);

	}
	//�����������ɼ����
	void SceneManager::ClipScene()
	{
		for(uint i=0; i<m_EntityList.size(); i++)
		{
			bool visible;
			EntityPtr const &ent = m_EntityList[i];
			if(!(ent->GetAttrib() & Entity::SOA_Overlay) && ent->GetVisible())
			{
				AABB bb_ws;
				if(ent->GetAttrib() & Entity::SOA_Cullable)
				{
					AABB ab;
					if(ent->GetAttrib() & Entity::SOA_Moveable)
					{
						AABB& box = ent->GetBound();
						Matrix4x4 mat = ent->GetEntityMatrix();
						Vector4 min, max;
						min = (ent->GetBound().min) * mat;
						max = (ent->GetBound().max) * mat;

						
						ab.min = min;
						ab.max = max;

					}
					else
					{
						ab = ent->GetBound();
					}
					visible = this->AABBVisible(ab);
				}
				visible = true;
			}
			else
			{
				visible = false;
			}
			//��ָ�� ������
		    m_VisMarkList[i] = visible;
		}

		{
			for(int i=0; i<m_VisMarkList.size(); ++i)
			{
				if(m_VisMarkList[i])
				{
					//m_pRenderQueue .AddEntity(m_EntityList[i]);
				}
			}
		}
	}
	//



}
