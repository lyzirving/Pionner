#pragma once

#include "gfx/renderer/ShaderUtils.h"

namespace pio
{
	class Shader;
	class RenderContext;

	namespace ShaderProcessor
	{
		static const char* MACRO_PRAGMA  = "pragma";
		static const char* MACRO_VERSION = "version";
		static const char* MACRO_INCLUDE = "include";
		static const char* MACRO_IFDEF   = "ifdef";
		static const char* MACRO_ELSE    = "else";
		static const char* MACRO_ENDIF   = "endif";

		static const char* MACRO_PRAGMA_STAGE = "stage";
		static const char* MACRO_PRAGMA_COLON = ":";
		static const char* MACRO_PRAGMA_ONECE = "once";

		static const char* MACRO_STAGE_VERT = "vert";
		static const char* MACRO_STAGE_FRAG = "frag";
		static const char* MACRO_STAGE_GEO  = "geo";

		// From https://wandbox.org/permlink/iXC7DWaU8Tk8jrf3 and is modified.
		enum class State : char { SlashOC, StarIC, SingleLineComment, MultiLineComment, NotAComment };

		template <typename InputIt, typename OutputIt>
		void CopyWithoutComments(InputIt first, InputIt last, OutputIt out)
		{
			State state = State::NotAComment;

			while (first != last)
			{
				switch (state)
				{
				case State::SlashOC:
					if (*first == '/') state = State::SingleLineComment;
					else if (*first == '*') state = State::MultiLineComment;
					else
					{
						state = State::NotAComment;
						*out++ = '/';
						*out++ = *first;
					}
					break;
				case State::StarIC:
					if (*first == '/') state = State::NotAComment;
					else state = State::MultiLineComment;
					break;
				case State::NotAComment:
					if (*first == '/') state = State::SlashOC;
					else *out++ = *first;
					break;
				case State::SingleLineComment:
					if (*first == '\n')
					{
						state = State::NotAComment;
						*out++ = '\n';
					}
					break;
				case State::MultiLineComment:
					if (*first == '*') state = State::StarIC;
					else if (*first == '\n') *out++ = '\n';
					break;
				}
				++first;
			}
		}
	}

	class ShaderCompiler : public Object
	{
		RTTR_ENABLE(Object)
	public:
		virtual ~ShaderCompiler() = default;

	public:
		static Ref<Shader> Compile(const Ref<RenderContext>& context, ShaderBuilder& builder);

	protected:
		virtual void ParseMetadata() = 0;
		virtual bool Preprocess(ShaderBuilder& builder) = 0;
		virtual Ref<Shader> CreateShader(const Ref<RenderContext>& context, const ShaderBuilder& builder) = 0;	

	private:
		static Ref<ShaderCompiler> Create(const Ref<RenderContext>& context);

	protected:
		std::string m_Path{};
		std::string m_Source{}, m_Name{};
		ShaderUtils::SourceLang m_Lang{ ShaderUtils::SourceLang::NONE };
		std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData> m_StageSource{};
	};
}