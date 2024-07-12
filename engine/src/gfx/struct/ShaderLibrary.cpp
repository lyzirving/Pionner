#include "ShaderLibrary.h"

#include "gfx/rhi/ShaderCompiler.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderLibrary"

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
		auto start = TimeUtil::CurrentTimeMs();
		auto* p = ShaderLibrary::Get();
		p->add(ShaderProgram::MaterialPreview, ShaderCompiler::Compile("shader/MterialPreview.glsl"));
		p->add(ShaderProgram::GeometryPass, ShaderCompiler::Compile("shader/GeometryPass.glsl"));
		p->add(ShaderProgram::LightingPass, ShaderCompiler::Compile("shader/LightingPass.glsl"));
		p->add(ShaderProgram::Sprite, ShaderCompiler::Compile("shader/Sprite.glsl"));
		p->add(ShaderProgram::SkyBox, ShaderCompiler::Compile("shader/SkyBox.glsl"));
		p->add(ShaderProgram::DistLightShadowMap, ShaderCompiler::Compile("shader/DistLightShadowMap.glsl"));
		p->add(ShaderProgram::PtLightShadowMap, ShaderCompiler::Compile("shader/PtLightShadowMap.glsl"));
		p->add(ShaderProgram::IBL_EquirecToCube, ShaderCompiler::Compile("shader/IBL_EquirectangularToCube.glsl"));
		p->add(ShaderProgram::IBL_DiffuseCnvl, ShaderCompiler::Compile("shader/IBL_DiffuseCnvl.glsl"));		
		p->add(ShaderProgram::IBL_BrdfConvl, ShaderCompiler::Compile("shader/IBL_BrdfCnvl.glsl"));
		auto end = TimeUtil::CurrentTimeMs();
		LOGD("take [%lu]ms time", (end - start));
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
			case ShaderType::TextureQuad:
			{
				m_shaders[index] = Shader::Create("TextureQuad", "quad2d", "quad2d");
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
		case ShaderProgram::MaterialPreview:
			return "MaterialPreview";
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
		case ShaderProgram::PtLightShadowMap:
			return "PtLightShadowMap";
		default:
			return "Unknown Shader Program";
		}
	}
}