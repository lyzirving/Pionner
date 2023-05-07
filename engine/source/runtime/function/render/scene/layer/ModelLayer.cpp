#include <algorithm>

#include "function/render/scene/layer/ModelLayer.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"
#include "function/render/entity/RenderEntity.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/cmd/DrawCmd.h"

#include "function/framework/world/World.h"
#include "function/framework/comp/RenderComp.h"

#include "function/render/RenderDef.h"

namespace Pionner
{
	ModelLayer::ModelLayer(const std::shared_ptr<Rhi> &rhi)
		: RenderLayer(rhi)
	{
	}

	ModelLayer::~ModelLayer()
	{
	}

	void ModelLayer::draw(RenderParam &param)
	{
		std::shared_ptr<World> world = param.world;
		std::shared_ptr<DrawCmd> drawCmd = param.rhi->getDrawCmd();

		world->iterate([&](decs::EntityID id, RenderComp &comp)
		{
			drawCmd->drawEntity(comp.m_entity, param);
		});

	}
}