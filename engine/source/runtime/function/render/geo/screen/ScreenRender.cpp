#include "ScreenRender.h"

#include "function/framework/comp/MeshComp.h"
#include "function/framework/comp/TransformComp.h"

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
#define LOCAL_TAG "ScreenRender"

namespace Pionner
{
	ScreenRender::ScreenRender(ShaderType shaderType, const std::string &name)
		: Geometry(name.c_str())
		, m_shaderType(shaderType), m_texId(0)
	{
	}

	ScreenRender::~ScreenRender() = default;

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

}