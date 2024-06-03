#ifndef __PIONNER_GFX_RHI_SHADER_UTILS_H__
#define __PIONNER_GFX_RHI_SHADER_UTILS_H__

#include "RhiDef.h"

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
			NONE, GLSL, HLSL
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
			std::unordered_set<IncludeData> Includers{};
		};

		inline SourceLang LanguageFromExtension(const std::string& type)
		{
			if (type == ".glsl") return SourceLang::GLSL;
			if (type == ".hlsl") return SourceLang::HLSL;

			return SourceLang::NONE;
		}

		inline const char* LanguageToString(SourceLang lang)
		{
			switch (lang)
			{
			case SourceLang::GLSL: return "glsl";
			case SourceLang::HLSL: return "hlsl";
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
	};
}

#endif