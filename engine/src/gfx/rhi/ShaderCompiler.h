#ifndef __PIONNER_GFX_RHI_SHADER_COMPILER_H__
#define __PIONNER_GFX_RHI_SHADER_COMPILER_H__

#include "Shader.h"

namespace pio
{

	namespace ShaderProcessor
	{
		static const char* MACRO_PRAGMA  = "pragma";
		static const char* MACRO_VERSION = "version";
		static const char* MACRO_INCLUDE = "include";

		static const char* MACRO_PROCESSOR_STAGE = "stage";
		static const char* MACRO_PROCESSOR_COLON = ":";

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

	class ShaderCompiler
	{
	public:
		virtual ~ShaderCompiler() = default;

	protected:
		virtual bool preprocess() = 0;

	public:
		static Ref<Shader> Compile(const std::string &path);
	};
}

#endif