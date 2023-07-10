#include "Plane.h"

#include "world/comp/MeshComp.h"

#include "render/RenderDef.h"
#include "render/rhi/Rhi.h"

#include "render/resource/ResourceDef.h"
#include "render/resource/buffer/GfxBuffer.h"
#include "render/resource/buffer/GfxFrameBuffer.h"
#include "render/resource/RenderResourceMgr.h"

#include "render/scene/SceneMgr.h"

#include "render/shader/ShaderMgr.h"

#include "core/math/MathLib.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Plane"

namespace pio
{
	Plane::Plane() : Geometry("Plane")
	{
		m_type = GEO_TYPE_PLANE;
		m_mesh = std::make_shared<MeshComp>();
		m_transform = std::make_shared<TransformComp>();
	}

	Plane::Plane(const std::string &name) : Geometry(name.c_str())
	{
		m_type = GEO_TYPE_PLANE;
		m_mesh = std::make_shared<MeshComp>();
		m_transform = std::make_shared<TransformComp>();
	}

	Plane::~Plane() = default;

	void Plane::draw(RenderParam &param)
	{
		if (!m_mesh || !m_transform)
		{
			LOG_ERR("invalid component");
			return;
		}

		initialize(param);

		std::shared_ptr<Shader> shader{ nullptr };
		if (!dealShader(param, shader))
			return;

		auto rhi = param.rhi;
		DepthTest depth = DepthTest::common();
		Blend blend = Blend::common();
		CullFace cull = CullFace::common();

		rhi->setCullMode(cull);
		rhi->setDepthMode(depth);
		rhi->setBlendMode(blend);

		auto cmd = param.rhi->getDrawCmd();
		cmd->drawGeometry(*this, param);

		if (param.sceneMgr->lightExist()) 
		{ 
			param.sceneMgr->selectedLight()->unbind(param);
		}

		shader->use(false);
	}

	void Plane::initialize(RenderParam &param)
	{
		if (!m_mesh->m_initialized)
		{
			float scale = 1.f;

			std::vector<Vertex> vArray{};
			Vertex v{};
			v.pos = glm::vec3(-scale, 0.f, -scale);
			v.normal = glm::vec3(0.f, 1.f, 0.f);
			v.texCoord = glm::vec2(0.f, 1.f);
			vArray.push_back(v);

			v.pos = glm::vec3(-scale, 0.f, scale);
			v.normal = glm::vec3(0.f, 1.f, 0.f);
			v.texCoord = glm::vec2(0.f, 0.f);
			vArray.push_back(v);

			v.pos = glm::vec3(scale, 0.f, -scale);
			v.normal = glm::vec3(0.f, 1.f, 0.f);
			v.texCoord = glm::vec2(1.f, 1.f);
			vArray.push_back(v);

			v.pos = glm::vec3(scale, 0.f, scale);
			v.normal = glm::vec3(0.f, 1.f, 0.f);
			v.texCoord = glm::vec2(1.f, 0.f);
			vArray.push_back(v);

			std::vector<uint32_t> indices{ 0, 1, 2, 2, 1, 3 };
			m_mesh->initialize(vArray, indices);
		}
	}

	bool Plane::dealShader(RenderParam &param, std::shared_ptr<Shader> &shader)
	{
		auto scene = param.sceneMgr;
		std::shared_ptr<Light> light{ nullptr };
		bool lightExist = scene->lightExist();
		if (lightExist) { light = scene->selectedLight(); }

		shader = param.shaderMgr->get(SHADER_TYPE_COLOR_GEOMETRY);

		if (!shader)
		{
			LOG_ERR("fail to get color geometry shader");
			return false;
		}

		shader->use(true);

		if (lightExist) { light->dealShader(param, shader, 1); }

		glm::mat4 modelMat = m_transform->getMat();
		shader->setMat4("u_modelMat", modelMat);
		shader->setMat3("u_normalMat", MathLib::normalMat(modelMat));

		shader->setMat4("u_viewMat", scene->m_camera->getViewMat());
		shader->setMat4("u_prjMat", scene->m_frustum->getPerspectMat());
		shader->setVec4("u_color", m_mesh->m_color);
		shader->setInt("u_calcShadow", lightExist ? 1 : 0);

		return true;
	}
}