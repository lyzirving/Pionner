#include "ShaderManager.h"
#include "render/shader/Shader.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderManager"

namespace pio
{
	namespace gfx
	{
		ShaderManager *ShaderManager::g_instance{ nullptr };
		std::mutex ShaderManager::    g_objMutex{};

		ShaderManager::ShaderManager() : m_shaders()
		{
		}

		ShaderManager::~ShaderManager() = default;

		ShaderManager *ShaderManager::self()
		{
			if (!g_instance)
			{
				std::lock_guard<std::mutex> guard{ g_objMutex };
				if (!g_instance)
				{
					g_instance = new ShaderManager;
				}
			}
			return g_instance;
		}

		std::shared_ptr<Shader> ShaderManager::get(ShaderType type)
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
							m_shaders[type] = std::make_shared<Shader>("mesh", "mesh", "mesh");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_LIGHTED_MESH:
						{
							m_shaders[type] = std::make_shared<Shader>("lighted_mesh", "lighted_mesh", "lighted_mesh");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_LIGHTED_NORM_MESH:
						{
							m_shaders[type] = std::make_shared<Shader>("lighted_nor_mesh", "lighted_nor_mesh", "lighted_nor_mesh");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_SHADOW_MAP:
						{
							m_shaders[type] = std::make_shared<Shader>("shadow_map", "shadow_map", "shadow_map");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_POINT_SHADOW_MAP:
						{
							m_shaders[type] = std::make_shared<Shader>("pt_shadow_map", "pt_shadow_map", "pt_shadow_map", "pt_shadow_map");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_DEPTH_SCREEN_2D:
						{
							m_shaders[type] = std::make_shared<Shader>("depth_screen2d", "depth_screen2d", "depth_screen2d");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_PT_SHADOW_SCREEN:
						{
							m_shaders[type] = std::make_shared<Shader>("pt_shadow_screen", "pt_shadow_screen", "pt_shadow_screen");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_INFINITE_GRID:
						{
							m_shaders[type] = std::make_shared<Shader>("infinite_grid", "infinite_grid", "infinite_grid");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_CIRCLE:
						{
							m_shaders[type] = std::make_shared<Shader>("2d_circle", "2d_circle", "2d_circle");
							m_shaders[type]->init();
							ret = m_shaders[type];
							break;
						}
						case SHADER_TYPE_COLOR_GEOMETRY:
						{
							m_shaders[type] = std::make_shared<Shader>("color_geometry", "color_geometry", "color_geometry");
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

		void ShaderManager::shutdown()
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
}