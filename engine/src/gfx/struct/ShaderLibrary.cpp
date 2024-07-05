#include "ShaderLibrary.h"

#include "gfx/rhi/ShaderCompiler.h"

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
		auto* p = ShaderLibrary::Get();
		p->add(ShaderProgram::GeometryPass, ShaderCompiler::Compile("shader/GeometryPass.glsl"));
		p->add(ShaderProgram::LightingPass, ShaderCompiler::Compile("shader/LightingPass.glsl"));
		p->add(ShaderProgram::Sprite, ShaderCompiler::Compile("shader/Sprite.glsl"));
		p->add(ShaderProgram::SkyBox, ShaderCompiler::Compile("shader/SkyBox.glsl"));
		p->add(ShaderProgram::DistLightShadowMap, ShaderCompiler::Compile("shader/DistLightShadowMap.glsl"));
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
			case ShaderType::LineSegment:
			{
				m_shaders[index] = Shader::Create("LineSegment", "line_segment", "line_segment");
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
			case ShaderType::Sprite:
			{
				m_shaders[index] = Shader::Create("Sprite", "quad2d", "sprite");
				return m_shaders[index];
			}
			case ShaderType::Outline_Deferred:
			{
				m_shaders[index] = Shader::Create("Outline_Deferred", "outline", "outline_deferred");
				return m_shaders[index];
			}
			case ShaderType::Equirectangular_To_Cube:
			{
				m_shaders[index] = Shader::Create("equirectangular_to_cube", "cube", "equirectangular_to_cube");
				return m_shaders[index];
			}
			case ShaderType::Diffuse_Convolution:
			{
				m_shaders[index] = Shader::Create("diffuse_convolution", "cube", "diffuse_convolution");
				return m_shaders[index];
			}
			case ShaderType::PrefilterMap_Convolution:
			{
				m_shaders[index] = Shader::Create("prefilter_map_convolution", "cube", "prefilter_map_convolution");
				return m_shaders[index];
			}
			case ShaderType::Brdf_Convolution:
			{
				m_shaders[index] = Shader::Create("brdf_convolution", "quad2d", "brdf_convolution");
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
			item.reset();
		}

		for (auto& item : m_shaderPrograms)
		{
			item.reset(); 
		}
	}

	const char* ShaderLibrary::ToStr(ShaderProgram type)
	{
		switch (type)
		{
		case ShaderProgram::GeometryPass:
			return "GeometryPass";
		case ShaderProgram::LightingPass:
			return "LightingPass";
		case ShaderProgram::Sprite:
			return "Sprite";
		case ShaderProgram::SkyBox:
			return "SkyBox";
		case ShaderProgram::DistLightShadowMap:
			return "DistLightShadowMap";
		default:
			return "Default Shader Program";
		}
	}
}