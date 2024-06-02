#ifndef __PIONNER_GFX_RHI_SHADER_UTILS_H__
#define __PIONNER_GFX_RHI_SHADER_UTILS_H__

#include "RhiDef.h"

namespace pio
{
	class ShaderUtils
	{
	public:
		enum class SourceLang
		{
			NONE, GLSL, HLSL
		};

		enum ShaderStageFlagBits
		{
			SHADER_STAGE_VERTEX_BIT = 1,
			SHADER_STAGE_TESSELLATION_CONTROL_BIT = 2,
			SHADER_STAGE_TESSELLATION_EVALUATION_BIT = 4,
			SHADER_STAGE_GEOMETRY_BIT = 8,
			SHADER_STAGE_FRAGMENT_BIT = 16,
			SHADER_STAGE_COMPUTE_BIT = 32,
			SHADER_STAGE_ALL_GRAPHICS = 0x0000001F,
			SHADER_STAGE_ALL = 0x7FFFFFFF
		};

		static SourceLang LanguageFromExtension(const std::string& type)
		{
			if (type == ".glsl") return SourceLang::GLSL;
			if (type == ".hlsl") return SourceLang::HLSL;

			return SourceLang::NONE;
		}

		static const char* LanguageToString(SourceLang lang)
		{
			switch (lang)
			{
			case SourceLang::GLSL: return "glsl";
			case SourceLang::HLSL: return "hlsl";
			default:
				return "none";
			}
		}

		static ShaderStageFlagBits ShaderStageFromString(const std::string_view &type)
		{
			if (type == "vert")	return SHADER_STAGE_VERTEX_BIT;
			if (type == "geo")	return SHADER_STAGE_GEOMETRY_BIT;
			if (type == "frag")	return SHADER_STAGE_FRAGMENT_BIT;
			if (type == "comp")	return SHADER_STAGE_COMPUTE_BIT;

			return SHADER_STAGE_ALL;
		}

		static std::string ReadShaderSource(const std::string& path);
	};
}

#endif