#include <algorithm>

#include "render/scene/layer/ModelLayer.h"
#include "render/scene/Camera.h"
#include "render/scene/Frustum.h"
#include "render/entity/RenderEntity.h"

#include "render/rhi/Rhi.h"
#include "render/rhi/DrawCmd.h"

#include "render/geo/InfiniteGrid.h"

#include "world/World.h"

#include "world/comp/RenderComp.h"
#include "world/comp/LightComp.h"
#include "world/comp/GeometryComp.h"

#include "render/RenderDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelLayer"

namespace pio
{
	ModelLayer::ModelLayer() : RenderLayer(), m_grid(new InfiniteGrid())
	{
	}

	ModelLayer::~ModelLayer()
	{
		m_grid.reset();
	}

	void ModelLayer::draw(RenderParam &param)
	{
		auto rhi = param.rhi;
		auto cmd = rhi->getDrawCmd();

		const RenderViewport &port = param.renderViewport;
		rhi->setViewport(port.m_left, port.m_top, port.m_width, port.m_height);

		std::shared_ptr<World> world = param.world;

		world->iterate([&](decs::EntityID id, RenderComp &comp)
		{
			if (comp.m_entity) comp.m_entity->draw(param);
		});
		
		world->iterate([&](decs::EntityID id, GeometryComp &comp)
		{
			comp.m_geometry->draw(param);
		});
		
		world->iterate([&](decs::EntityID id, LightComp &comp)
		{
			cmd->drawEntity(*comp.m_obj, param);
		});
		
		m_grid->draw(param);
	}
}