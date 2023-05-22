#include "function/render/shader/ShaderMgr.h"
#include "function/render/shader/Shader.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/ShaderRhi.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderMgr"

namespace Pionner
{
	ShaderMgr::ShaderMgr() : m_shaders()
	{
	}

	ShaderMgr::~ShaderMgr()
	{
	}

	std::shared_ptr<Shader> ShaderMgr::get(ShaderType type, const std::shared_ptr<Rhi> &rhi)
	{
		std::shared_ptr<Shader> ret{ nullptr };
		if (type >= 0 && type < SHADER_TYPE_CNT)
		{
			if (!m_shaders[type].get())
			{
				switch (type)
				{
					case Pionner::SHADER_TYPE_MESH:
					{
						m_shaders[type] = std::make_shared<Shader>(rhi->getShaderRhi(), "mesh", "mesh", "mesh");
						m_shaders[type]->init();
						ret = m_shaders[type];
						break;
					}
					case Pionner::SHADER_TYPE_CNT:
					default:
					{
						LOG_ERR("shader type[%u] is not implemented", type);
						break;
					}
				}
			}
			else
			{
				ret = m_shaders[type];
			}
		}
		else
		{
			LOG_ERR("invalid type[%u]", type);
		}
		return ret;
	}

	void ShaderMgr::shutdown()
	{
		for (int i = 0; i < SHADER_TYPE_CNT; i++)
		{
			if (m_shaders[i])
			{
				m_shaders[i]->destroy();
				m_shaders[i].reset();
			}
		}
	}
}