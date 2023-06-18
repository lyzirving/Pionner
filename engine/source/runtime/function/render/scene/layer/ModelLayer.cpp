#include <algorithm>

#include "function/render/scene/layer/ModelLayer.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"
#include "function/render/entity/RenderEntity.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/DrawCmd.h"

#include "function/render/geo/InfiniteGrid.h"

#include "function/framework/world/World.h"

#include "function/framework/comp/RenderComp.h"
#include "function/framework/comp/LightComp.h"

#include "function/render/RenderDef.h"

namespace Pionner
{
	ModelLayer::ModelLayer(const std::shared_ptr<Rhi> &rhi)
		: RenderLayer(rhi)
		, m_grid(new InfiniteGrid())
	{
	}

	ModelLayer::~ModelLayer()
	{
		m_grid.reset();
	}

	void ModelLayer::draw(RenderParam &param)
	{
		std::shared_ptr<World> world = param.world;

		world->iterate([&](decs::EntityID id, RenderComp &comp)
		{
			if (comp.m_entity) comp.m_entity->draw(param);
		});

		/*world->iterate([&](decs::EntityID id, LightComp &comp)
		{

		});*/

		m_grid->draw(param);
	}
}