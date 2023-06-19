#include "ScreenRender.h"

#include "function/framework/comp/MeshComp.h"

#include "function/render/RenderDef.h"
#include "function/render/rhi/Rhi.h"

#include "function/render/resource/ResourceDef.h"
#include "function/render/resource/buffer/GfxBuffer.h"
#include "function/render/resource/RenderResourceMgr.h"

#include "function/render/shader/ShaderMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelEntity"

namespace Pionner
{
	ScreenRender::ScreenRender(ShaderType shaderType) : Geometry("Screen Render"), m_texId(0), m_shaderType(shaderType)
	{
	}

	ScreenRender::~ScreenRender() = default;

	void ScreenRender::draw(RenderParam &param)
	{
		initialize(param);

		std::shared_ptr<Shader> shader{ nullptr };
		if (!dealShader(param, shader))
			return;

		auto rhi = param.rhi;
		const RenderViewport &port = param.renderViewport;

		rhi->setViewport(port.m_left, port.m_top, port.m_width, port.m_height);
		rhi->clearColor(glm::vec4(1.f));
		rhi->clear(COLOR_BUF_BIT | DEPTH_BUF_BIT);

		DepthTest depthTest{};
		depthTest.m_enbale = true;
		rhi->setDepthMode(depthTest);

		// temporary buffer that holds an initialized buffer
		auto holderTex = param.resource->createHolderBuffer(BUF_TEXTURE);
		holderTex->setHolderId(m_texId);

		shader->use(true);

		holderTex->bindTarget(0);
		shader->setInt("u_depSampler", m_texId);

		auto drawCmd = param.rhi->getDrawCmd();
		drawCmd->drawGeometry(*this, param);

		holderTex->unbind();

		depthTest.m_enbale = false;
		rhi->setDepthMode(depthTest);

		shader->use(false);
	}

	void ScreenRender::initialize(RenderParam &param)
	{
		if (!m_mesh || !m_mesh->m_initialized)
		{
			m_mesh = std::make_shared<MeshComp>();

			std::vector<Vertex> vArray{};
			Vertex v{};
			v.pos = glm::vec3(-1.f, 1.f, 0.f);
			v.normal = glm::vec3(0.f);
			v.texCoord = glm::vec2(0.f, 1.f);
			vArray.push_back(v);

			v.pos = glm::vec3(-1.f, -1.f, 0.f);
			v.texCoord = glm::vec2(0.f, 0.f);
			vArray.push_back(v);

			v.pos = glm::vec3(1.f, 1.f, 0.f);
			v.texCoord = glm::vec2(1.f, 1.f);
			vArray.push_back(v);

			v.pos = glm::vec3(1.f, -1.f, 0.f);
			v.texCoord = glm::vec2(1.f, 0.f);
			vArray.push_back(v);

			std::vector<uint32_t> indices{ 0, 1, 2, 2, 1, 3 };
			m_mesh->initialize(vArray, indices);
		}
	}

	bool ScreenRender::dealShader(RenderParam &param, std::shared_ptr<Shader> &shader)
	{
		shader = param.shaderMgr->get(m_shaderType, param.rhi);
		if (!shader || !shader->isInit())
		{
			LOG_ERR("depth screen 2d shader is invalid");
			goto fail;
		}
		return true;
	fail:

		return false;
	}

}