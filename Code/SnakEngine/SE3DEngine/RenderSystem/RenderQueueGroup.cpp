#include "RenderQueueGroup.h"

namespace SE
{
	RenderQueueGroup::RenderQueueGroup(void)
	{
	}


	RenderQueueGroup::~RenderQueueGroup(void)
	{

	}
	void RenderQueueGroup::AddRenderObject(Entity& entity)
	{
		 PriorityList.push_back(entity);
	}
	void RenderQueueGroup::DelRenderObject()
	{}
	void RenderQueueGroup::DistanceSort()
	{}
}