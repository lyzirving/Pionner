#include "ShaderLibrary.h"

namespace pio
{
	PIO_SINGLETON_IMPL(ShaderLibrary)

	ShaderLibrary::ShaderLibrary()
	{
	}

	ShaderLibrary::~ShaderLibrary()
	{
	}

	void ShaderLibrary::Init()
	{
		ShaderLibrary::Get();
	}

	void ShaderLibrary::Shutdown()
	{
		{
			std::lock_guard<std::mutex> lk{ s_mutex };
			if (s_instance)
				s_instance->release();
			delete s_instance;
			s_instance = nullptr;
		}
	}

	Ref<Shader> ShaderLibrary::find(ShaderType type)
	{
		uint32_t index = (uint32_t)type;
		if (m_shaders[index])
			return m_shaders[index];

		switch (type)
		{
			case ShaderType::PBR_Mesh:
			{
				m_shaders[index] = Shader::Create("PBR_Mesh", "mesh", "mesh_pbr");
				return m_shaders[index];
			}
			case ShaderType::DistantLight_ShadowData:
			{
				m_shaders[index] = Shader::Create("DistantLight_ShadowData", "mesh_shadow_distant_light", "mesh_shadow_distant_light");
				return m_shaders[index];
			}
			case ShaderType::PointLight_ShadowData:
			{
				m_shaders[index] = Shader::Create("PointLight_ShadowData", "mesh_shadow_point_light", "mesh_shadow_point_light", "mesh_shadow_point_light");
				return m_shaders[index];
			}
			case ShaderType::MaterialPreview:
			{
				m_shaders[index] = Shader::Create("MaterialPreview", "mesh", "mesh_material_preview");
				return m_shaders[index];
			}
			case ShaderType::Wireframe:
			{
				m_shaders[index] = Shader::Create("Wireframe", "mesh", "mesh_wireframe");
				return m_shaders[index];
			}
			case ShaderType::LightVolume:
			{
				m_shaders[index] = Shader::Create("LightVolume", "mesh_light_volume", "mesh_empty");
				return m_shaders[index];
			}
			case ShaderType::Mesh_Deferred:
			{
				m_shaders[index] = Shader::Create("Mesh_Deferred", "mesh", "mesh_deferred");
				return m_shaders[index];
			}
			case ShaderType::DistantLighting_Deferred:
			{
				m_shaders[index] = Shader::Create("DistantLighting_Deferred", "quad2d", "lighting_distant_deferred");
				return m_shaders[index];
			}
			case ShaderType::LightingEffect_Deferred:
			{
				m_shaders[index] = Shader::Create("LightingEffect_Deferred", "quad2d", "lighting_deferred");
				return m_shaders[index];
			}			
			case ShaderType::Color_Line:
			{
				m_shaders[index] = Shader::Create("ColorLine", "color_line", "color_line");
				return m_shaders[index];
			}
			case ShaderType::Outline:
			{
				m_shaders[index] = Shader::Create("Outline", "outline", "outline");
				return m_shaders[index];
			}
			case ShaderType::TextureQuad:
			{
				m_shaders[index] = Shader::Create("TextureQuad", "quad2d", "quad2d");
				return m_shaders[index];
			}			
			case ShaderType::Outline_Deferred:
			{
				m_shaders[index] = Shader::Create("Outline_Deferred", "outline", "outline_deferred");
				return m_shaders[index];
			}
			case ShaderType::Equirectangular_To_Cube:
			{
				m_shaders[index] = Shader::Create("equirectangular_to_cube", "equirectangular_to_cube", "equirectangular_to_cube");
				return m_shaders[index];
			}
			case ShaderType::Skybox:
			{
				m_shaders[index] = Shader::Create("skybox", "skybox", "skybox");
				return m_shaders[index];
			}
			case ShaderType::Postprocessing:
			{
				m_shaders[index] = Shader::Create("postprocessing", "quad2d", "postprocessing");
				return m_shaders[index];
			}
			default:
				return Ref<Shader>();
		}
	}

	void ShaderLibrary::release()
	{
		for (auto &item : m_shaders)
		{
			if (item)
				item->destroy();
			item.reset();
		}
	}
}