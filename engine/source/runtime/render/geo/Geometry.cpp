#include "render/geo/Geometry.h"

#include "world/comp/MeshComp.h"
#include "world/comp/TransformComp.h"

#include "render/RenderDef.h"
#include "render/scene/SceneMgr.h"
#include "render/shader/ShaderMgr.h"

#include "render/scene/light/PointLight.h"

#include "Plane.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Geometry"

namespace pio
{
	std::shared_ptr<Geometry> Geometry::createGeometry(GeometryType type)
	{
		std::shared_ptr<Geometry> result{ nullptr };
		switch (type)
		{
			case pio::GEO_TYPE_PLANE:
				result = std::shared_ptr<Geometry>(new Plane);
				break;
			default:
				break;
		}
		return result;
	}

	Geometry::Geometry(const char *name)
		: m_type(GEO_TYPE_NONE), m_name(name)
		, m_mesh(nullptr), m_transform(nullptr)
	{
	}

	Geometry::~Geometry()
	{
		m_mesh.reset();
		m_transform.reset();
	}

	bool Geometry::dealDepthShader(RenderParam &param, /*out*/std::shared_ptr<Shader> &shader)
	{
		if (!m_mesh || !m_mesh->m_initialized)
		{
			LOG_ERR("geometry[%s]'s mesh is invalid", m_name.c_str());
			return false;
		}
		if (!m_transform)
		{
			LOG_ERR("geometry[%s]'s transform is invalid", m_name.c_str());
			return false;
		}

		auto scene = param.sceneMgr;
		LightType curLight = scene->m_curLight;
		auto light = scene->m_lights[curLight];
		if (!light)
		{
			LOG_ERR("geometry[%s] fail to get light[%u]", m_name.c_str(), curLight);
			return false;
		}

		if (curLight == LIGHT_TYPE_DIRECTIONAL)
		{
			return dealDirectionLightShader(param, shader);
		}
		else if (curLight == LIGHT_TYPE_POINT)
		{
			return dealPointLightShader(param, shader);
		}
		else
		{
			LOG_ERR("geometry[%s] other type[%u] light is not supported now", m_name.c_str(), curLight);
			return false;
		}
	}

	bool Geometry::dealDirectionLightShader(RenderParam &param, /*out*/std::shared_ptr<Shader> &shader)
	{
		shader = param.shaderMgr->get(SHADER_TYPE_SHADOW_MAP, param.rhi);
		if (!shader)
		{
			LOG_ERR("geometry[%s] fail to get shadow map shader", m_name.c_str());
			return false;
		}

		auto scene = param.sceneMgr;
		LightType curLight = scene->m_curLight;
		auto light = scene->m_lights[curLight];

		shader->use(true);
		shader->setMat4("u_lightViewMat", light->getViewMat());
		shader->setMat4("u_lightPrjMat", light->getPrjMat());
		shader->setMat4("u_modelMat", m_transform->getMat());
		shader->setInt("u_calcShadow", 0);

		return true;
	}

	bool Geometry::dealPointLightShader(RenderParam &param, /*out*/std::shared_ptr<Shader> &shader)
	{
		auto scene = param.sceneMgr;
		LightType curLight = scene->m_curLight;

		auto light = scene->m_lights[curLight];
		auto pPtLight = light->to<PointLight>();
		if (!pPtLight)
		{
			LOG_ERR("fail to convert to PointLight ptr");
			return false;
		}

		shader = param.shaderMgr->get(SHADER_TYPE_POINT_SHADOW_MAP, param.rhi);
		if (!shader)
		{
			LOG_ERR("geometry[%s] fail to get point shadow map shader", m_name.c_str());
			return false;
		}
		
		shader->use(true);

		shader->setMat4("u_modelMat", m_transform->getMat());
		shader->setVec3("u_lightPos", light->position());
		shader->setFloat("u_farPlane", light->far());
		for (uint32_t i = 0; i < 6; ++i)
		{
			LightDir dir = LightDir(i);
			shader->setMat4("u_lightMats[" + std::to_string(i) + "]", pPtLight->getLightMat(dir));
		}
		return true;
	}

	bool Geometry::isInitialized()
	{
		return false;
	}

	void Geometry::translate(float x, float y, float z)
	{
		if (m_transform)
			m_transform->translate(x, y, z);
	}

	void Geometry::rotate(float angle, float x, float y, float z)
	{
		if (m_transform)
			m_transform->rotate(angle, x, y, z);
	}

	template<class T>
	bool Geometry::is()
	{
		return false;
	}

	template<class T>
	T *Geometry::as()
	{
		return nullptr;
	}

}