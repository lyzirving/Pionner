#ifndef __PIONNER_GFX_RHI_SHADER_UTILS_H__
#define __PIONNER_GFX_RHI_SHADER_UTILS_H__

#include "gfx/GfxDef.h"

namespace pio
{
	struct IncludeData
	{
		std::string FilePath{};
		std::size_t LineStart{}, LineEnd{};

		bool operator==(const IncludeData& other) const noexcept
		{
			return this->FilePath == other.FilePath;
		}
	};
}

namespace std
{
	template<>
	struct hash<pio::IncludeData>
	{
		size_t operator()(const pio::IncludeData& data) const noexcept
		{
			return std::hash<std::string>()(data.FilePath);
		}
	};
}

namespace pio
{
	namespace ShaderUtils
	{
		enum class SourceLang
		{
			NONE, GLSL, HLSL, GLSLH
		};

		enum ShaderStageFlagBits
		{
			SHADER_STAGE_VERTEX_BIT = 1,
			SHADER_STAGE_GEOMETRY_BIT = 2,
			SHADER_STAGE_FRAGMENT_BIT = 4,
			SHADER_STAGE_COMPUTE_BIT = 8,
			SHADER_STAGE_ALL = 0x7FFFFFFF
		};

		struct StageData
		{
			ShaderStageFlagBits Stage{ SHADER_STAGE_ALL };
			std::string Source{};
			std::vector<IncludeData> Includers{};
		};

		inline SourceLang LanguageFromExtension(const std::string& type)
		{
			if (type == ".glsl")  return SourceLang::GLSL;
			if (type == ".hlsl")  return SourceLang::HLSL;
			if (type == ".glslh") return SourceLang::GLSLH;

			return SourceLang::NONE;
		}

		inline const char* LanguageToString(SourceLang lang)
		{
			switch (lang)
			{
			case SourceLang::GLSL:  return "glsl";
			case SourceLang::HLSL:  return "hlsl";
			case SourceLang::GLSLH: return "glslh";
			default:
				return "none";
			}
		}

		inline ShaderStageFlagBits ShaderStageFromString(const std::string_view& type)
		{
			if (type == "vert")	return SHADER_STAGE_VERTEX_BIT;
			if (type == "geo")	return SHADER_STAGE_GEOMETRY_BIT;
			if (type == "frag")	return SHADER_STAGE_FRAGMENT_BIT;
			if (type == "comp")	return SHADER_STAGE_COMPUTE_BIT;

			return SHADER_STAGE_ALL;
		}

		inline const char *GetShaderPath(ShaderType type)
		{
			switch(type)
			{
				case ShaderType::GBuffer:
					return "shader/GBufferPass.glsl";
				case ShaderType::Deferred:
					return "shader/DeferredPass.glsl";
				case ShaderType::ColorCorrect:
					return "shader/ColorCorrect.glsl";
				default:
					#ifdef LOCAL_TAG
					#undef LOCAL_TAG
					#endif
					#define LOCAL_TAG "ShaderUtils"
					LOGE("err! invalid shader spec[%u]", type);
					std::abort();
					return "Invalid shader spec";
			}
		}
	};
}

#endif