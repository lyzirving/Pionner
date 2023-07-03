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
#include "function/framework/comp/GeometryComp.h"

#include "function/render/RenderDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelLayer"

namespace Pionner
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