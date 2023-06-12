#include "DepthPass.h"

#include "function/framework/world/World.h"

#include "function/framework/comp/RenderComp.h"
#include "function/framework/comp/LightComp.h"
#include "function/framework/comp/OcclusionComp.h"

#include "function/render/scene/SceneMgr.h"

#include "function/render/rhi/Rhi.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DepthPass"

namespace Pionner
{
	DepthPass::DepthPass() : RenderPassBase()
	{
	}

	DepthPass::~DepthPass()
	{
	}

	void DepthPass::initialize(const RenderPassInitInfo &info)
	{
		m_rhi = info.rhi;
	}

	void DepthPass::shutdown()
	{
		m_rhi.reset();
	}

	void DepthPass::draw(RenderParam &param)
	{
		auto world = param.world;
		auto cmd = param.rhi->getDrawCmd();
		auto camera = param.sceneMgr->m_camera;

		std::shared_ptr<Entity> light{ nullptr };

		if (!(light = world->getEntity(World::ENTITY_POINT_LIGHT)) || !light->hasComp<LightComp>())
		{
			LOG_ERR("no light entity or comp");
			return;
		}

		LightComp &lightComp = light->getComp<LightComp>();
		if (!lightComp.m_light)
		{
			LOG_ERR("point light is invalid");
			return;
		}

		camera->restoreState();
		camera->setPosition(lightComp.m_light->position());

		//TODO: 1.Frustum.ortho; 2.abstract FrameBuffer and depth texture.

		world->iterate([&](decs::EntityID id, RenderComp &comp0, OcclusionComp &comp1)
		{

		});

		camera->popState();
	}
}