#include "DepthPass.h"

#include "world/World.h"

#include "world/comp/RenderComp.h"
#include "world/comp/GeometryComp.h"
#include "world/comp/LightComp.h"
#include "world/comp/ShadowComp.h"

#include "render/scene/SceneMgr.h"
#include "render/rhi/Rhi.h"

#include "render/geo/screen/DepthScreenRender.h"
#include "render/geo/screen/PointShadowScreenRender.h"
#include "render/shader/ShaderMgr.h"

#include "render/resource/RenderResourceMgr.h"
#include "render/resource/buffer/GfxFrameBuffer.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DepthPass"

namespace pio
{
	DepthPass::DepthPass() : RenderPassBase(), m_depthScreenRender(nullptr), m_ptShadowScreenRender(nullptr)
	{
	}

	DepthPass::~DepthPass()
	{
		m_depthScreenRender.reset();
		m_ptShadowScreenRender.reset();
	}

	void DepthPass::initialize(const RenderPassInitInfo &info)
	{
		m_rhi = info.rhi;
		//m_depthScreenRender = std::shared_ptr<ScreenRender>(new DepthScreenRender);
		//m_ptShadowScreenRender = std::shared_ptr<ScreenRender>(new PointShadowScreenRender);
	}

	void DepthPass::shutdown()
	{
		m_rhi.reset();
	}

	void DepthPass::draw(RenderParam &param)
	{
		auto rhi = param.rhi;
		auto cmd = rhi->getDrawCmd();
		auto world = param.world;
		std::shared_ptr<Shader> shader{ nullptr };
		const RenderViewport &port = param.renderViewport;

		auto scene = param.sceneMgr;
		auto light = scene->m_lights[scene->m_curLight];
		std::shared_ptr<GfxFrameBuffer> depthBuf{ nullptr };

		if (!light || !(depthBuf = light->getDepthFbo()) || depthBuf->getWidth() == 0 || depthBuf->getHeight() == 0)
		{
			return;
		}

		depthBuf->upload();

		depthBuf->bind();

		rhi->clear(DEPTH_BUF_BIT);
		rhi->setViewport(0, 0, depthBuf->getWidth(), depthBuf->getHeight());

		DepthTest depthTest = DepthTest::common();
		rhi->setDepthMode(depthTest);

		CullFace cull = CullFace::common();
		// Solve Perter panning, but the model has some problem.
		cull.m_mode = CULL_FRONT;
		rhi->setCullMode(cull);

		world->iterate([&](decs::EntityID id, ShadowComp &comp)
		{
			auto entity = world->getEntity(id.index);
		if (entity)
		{
			if (entity->hasComp<RenderComp>())
			{
				auto &renderComp = entity->getComp<RenderComp>();
				cmd->drawDepth(*renderComp.m_entity.get(), param);
			}
			else if (entity->hasComp<GeometryComp>())
			{
				auto &geoComp = entity->getComp<GeometryComp>();
				cmd->drawGeometryDepth(*geoComp.m_geometry.get(), param);
			}
		}
		});

		depthBuf->unbind();

		if (m_depthScreenRender)
		{
			m_depthScreenRender->setTextureId(depthBuf->getAttachment(DEPTH_ATTACH));
			m_depthScreenRender->draw(param);
		}

		if (m_ptShadowScreenRender)
		{
			m_ptShadowScreenRender->setTextureId(depthBuf->getAttachment(DEPTH_ATTACH));
			m_ptShadowScreenRender->draw(param);
		}
	}
}