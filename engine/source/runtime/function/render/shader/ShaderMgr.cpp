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
					case SHADER_TYPE_MESH:
					{
						m_shaders[type] = std::make_shared<Shader>(rhi->getShaderRhi(), "mesh", "mesh", "mesh");
						m_shaders[type]->init();
						ret = m_shaders[type];
						break;
					}
					case SHADER_TYPE_LIGHTED_MESH:
					{
						m_shaders[type] = std::make_shared<Shader>(rhi->getShaderRhi(), "lighted_mesh", "lighted_mesh", "lighted_mesh");
						m_shaders[type]->init();
						ret = m_shaders[type];
						break;
					}
					case SHADER_TYPE_SHADOW_MAP:
					{
						m_shaders[type] = std::make_shared<Shader>(rhi->getShaderRhi(), "shadow_map", "shadow_map", "shadow_map");
						m_shaders[type]->init();
						ret = m_shaders[type];
						break;
					}
					case SHADER_TYPE_DEPTH_SCREEN_2D:
					{
						m_shaders[type] = std::make_shared<Shader>(rhi->getShaderRhi(), "depth_screen2d", "depth_screen2d", "depth_screen2d");
						m_shaders[type]->init();
						ret = m_shaders[type];
						break;
					}
					case SHADER_TYPE_INFINITE_GRID:
					{
						m_shaders[type] = std::make_shared<Shader>(rhi->getShaderRhi(), "infinite_grid", "infinite_grid", "infinite_grid");
						m_shaders[type]->init();
						ret = m_shaders[type];
						break;
					}
					case SHADER_TYPE_CIRCLE:
					{
						m_shaders[type] = std::make_shared<Shader>(rhi->getShaderRhi(), "2d_circle", "2d_circle", "2d_circle");
						m_shaders[type]->init();
						ret = m_shaders[type];
						break;
					}
					case SHADER_TYPE_COLOR_GEOMETRY:
					{
						m_shaders[type] = std::make_shared<Shader>(rhi->getShaderRhi(), "color_geometry", "color_geometry", "color_geometry");
						m_shaders[type]->init();
						ret = m_shaders[type];
						break;
					}
					case SHADER_TYPE_CNT:
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