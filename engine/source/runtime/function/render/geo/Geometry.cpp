#include "function/render/geo/Geometry.h"

#include "function/framework/comp/MeshComp.h"
#include "function/framework/comp/TransformComp.h"

#include "function/render/RenderDef.h"
#include "function/render/scene/SceneMgr.h"
#include "function/render/shader/ShaderMgr.h"

#include "Plane.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Geometry"

namespace Pionner
{
	std::shared_ptr<Geometry> Geometry::createGeometry(GeometryType type)
	{
		std::shared_ptr<Geometry> result{ nullptr };
		switch (type)
		{
			case Pionner::GEO_TYPE_PLANE:
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
		shader = param.shaderMgr->get(SHADER_TYPE_SHADOW_MAP, param.rhi);
		if (!shader)
		{
			LOG_ERR("geometry[%s] fail to get shadow map shader", m_name.c_str());
			return false;
		}
		if (curLight == LIGHT_TYPE_DIRECTIONAL)
		{
			shader->use(true);
			shader->setMat4("u_lightViewMat", light->getViewMat());
			shader->setMat4("u_lightPrjMat", light->getPrjMat());
			shader->setMat4("u_modelMat", m_transform->getMat());
			shader->setInt("u_calcShadow", 0);
		}
		else
		{
			LOG_ERR("geometry[%s] other type[%u] light is not supported now", m_name.c_str(), curLight);
			return false;
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