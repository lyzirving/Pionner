#include "DepthScreenRender.h"

#include "render/RenderDef.h"
#include "render/rhi/Rhi.h"

#include "render/resource/ResourceDef.h"
#include "render/resource/buffer/GfxBuffer.h"
#include "render/resource/RenderResourceMgr.h"

#include "render/shader/ShaderMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DepthScreenRender"

namespace pio
{
	DepthScreenRender::DepthScreenRender() : ScreenRender(SHADER_TYPE_DEPTH_SCREEN_2D, "depth screen render")
	{
	}

	DepthScreenRender::~DepthScreenRender() = default;

	void DepthScreenRender::draw(RenderParam &param)
	{
		if (m_texId == 0)
		{
			LOG_ERR("depth screen render has invalid texture id");
			return;
		}

		initialize(param);

		std::shared_ptr<Shader> shader{ nullptr };
		if (!dealShader(param, shader))
		{
			LOG_ERR("depth screen render fail to deal shader");
			return;
		}

		auto rhi = param.rhi;
		const RenderViewport &port = param.renderViewport;

		rhi->setViewport(port.m_left, port.m_top, port.m_width, port.m_height);
		rhi->clear(COLOR_BUF_BIT | DEPTH_BUF_BIT);

		gfx::DepthTest depthtest = gfx::DepthTest::common();
		rhi->setDepthMode(depthtest);

		// temporary buffer that holds an initialized buffer
		auto holdertex = param.resource->createHolderBuffer(BUF_TEXTURE);
		holdertex->setHolderId(m_texId);

		shader->use(true);

		holdertex->bindTarget(0);
		shader->setInt("u_depSampler", 0);

		auto drawcmd = param.rhi->getDrawCmd();
		drawcmd->drawGeometry(*((Geometry *)this), param);

		holdertex->unbind();

		shader->use(false);
	}

	bool DepthScreenRender::dealShader(RenderParam &param, std::shared_ptr<Shader> &shader)
	{
		shader = param.shaderMgr->get(m_shaderType);
		return shader && shader->isInit();
	}
}