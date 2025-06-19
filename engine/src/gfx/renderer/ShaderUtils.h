#pragma once

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

		inline std::string ShaderIncludePath()
		{
			auto path = Path::ShaderRoot();
			path.append(Path::FILE_SEPARATOR)
				.append("include")
				.append(Path::FILE_SEPARATOR);
			return path;
		}

		inline ShaderStageFlagBits ShaderStageFromString(const std::string_view& type)
		{
			if (type == "vert")	return SHADER_STAGE_VERTEX_BIT;
			if (type == "geo")	return SHADER_STAGE_GEOMETRY_BIT;
			if (type == "frag")	return SHADER_STAGE_FRAGMENT_BIT;
			if (type == "comp")	return SHADER_STAGE_COMPUTE_BIT;

			return SHADER_STAGE_ALL;
		}

		inline const char* GetName(ShaderSpecifier spec)
		{
			switch(spec)
			{
				case SHADER_SPEC_MAIN_LIT_SHADOW_CASTER:
					return "MainLitShadowCaster";
				case SHADER_SPEC_POINT_LIT_SHADOW_CASTER:
					return "PointLitShadowCaster";
				case SHADER_SPEC_STANDARD:
					return "Standard";
				case SHADER_SPEC_DEFERRED:
					return "Deferred";
				case SHADER_SPEC_POSTPROCESS:
					return "PostProcess";
				case SHADER_SPEC_TEXTURED:
					return "Textured";
				case SHADER_SPEC_FORWARD:
					return "Forward";
				default:
				#ifdef LOCAL_TAG
				#undef LOCAL_TAG
				#endif
				#define LOCAL_TAG "ShaderUtils"
					LOGE("err! invalid shader spec[%u]", spec);
					std::abort();
					return "unknown shader spec";
			}
		}		
	};

	struct ShaderStageInfo
	{
		std::set<std::string> Macro;
	};

	class ShaderBuilder
	{			
	public:
		ShaderBuilder() {}
		~ShaderBuilder() = default;

		ShaderBuilder& SetPath(const std::string& path);
		ShaderBuilder& SetName(const std::string& name);
		ShaderBuilder& SetMutant(bool val = true);
		ShaderBuilder& AddMacro(const std::string& macro);
		ShaderBuilder& AddMacro(ShaderUtils::ShaderStageFlagBits stage, const std::string& macro);
		bool IsMarcoChange() const { return m_bMacroChange; }
		bool IsMutant() const { return m_bMutant; }

		std::string GetFullPath() const;
		const std::string& GetPath() const { return m_Path; }
		const std::string& GetName() const { return m_Name; }

		std::map<int32_t, ShaderStageInfo>& GetStageInfo() { return m_StageInfo; }
		const std::map<int32_t, ShaderStageInfo>& GetStageInfo() const { return m_StageInfo; }
		
	private:
		std::map<int32_t, ShaderStageInfo> m_StageInfo{};
		std::string m_Path{};
		std::string m_Name{};
		bool m_bMacroChange{ false };
		bool m_bMutant{ false };
	};

	namespace ShaderUtils
	{
		inline void AddMacros(uint32_t flags, ShaderBuilder& builder)
		{
			if(flags & ((uint32_t)ShaderPermutationFlag::USE_SKELETON))
			{
				builder.AddMacro(ShaderMacro::Get(ShaderPermutationFlag::USE_SKELETON));
			}

			if(flags & ((uint32_t)ShaderPermutationFlag::USE_CLEARCOAT))
			{
				builder.AddMacro(ShaderMacro::Get(ShaderPermutationFlag::USE_CLEARCOAT));
			}
		}
	}
}