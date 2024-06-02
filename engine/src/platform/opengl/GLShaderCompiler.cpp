#include "GLShaderCompiler.h"
#include "GLShader.h"

#include <spdlog/fmt/fmt.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLShaderCompiler"

namespace pio
{
	GLShaderCompiler::GLShaderCompiler() : ShaderCompiler()
	{
	}

	bool GLShaderCompiler::preprocess()
	{
		switch (m_lang)
		{
		case ShaderUtils::SourceLang::GLSL:
			return preprocessGLSL();
		default:
			break;
		}
		return false;
	}

	Ref<Shader> GLShaderCompiler::DoCompile(const std::string& path)
	{
		GLShaderCompiler compiler{};
		compiler.m_path = path;
		compiler.m_source = ShaderUtils::ReadShaderSource(path);
		compiler.parseMetadata();
		LOGD("begin to compile shader[%s, %s] from[%s]", compiler.m_name.c_str(), 
			ShaderUtils::LanguageToString(compiler.m_lang), compiler.m_path.c_str());

		if (!compiler.preprocess())
		{
			LOGE("err! preprocess fail");
			return Ref<Shader>();
		}

		return Ref<Shader>();
	}

	void GLShaderCompiler::parseMetadata()
	{
		size_t found = m_path.find_last_of("/");
		m_name = (found != std::string::npos) ? m_path.substr(found + 1) : m_path;
		found = m_name.find_last_of('.');
		std::string extention = (found != std::string::npos) ? m_name.substr(found) : m_name;
		m_lang = ShaderUtils::LanguageFromExtension(extention);
	}

	bool GLShaderCompiler::preprocessGLSL()
	{
		std::map<ShaderUtils::ShaderStageFlagBits, std::string> stageSource;
		if (!preprocessShader(stageSource))
		{
			LOGE("err! preprocess shader failed!");
			return false;
		}
		return true;
	}

	bool GLShaderCompiler::preprocessShader(std::map<ShaderUtils::ShaderStageFlagBits, std::string>& stageSource)
	{
		std::stringstream sourceStream;
		// clear comments in shader
		ShaderProcessor::CopyWithoutComments(m_source.begin(), m_source.end(), std::ostream_iterator<char>(sourceStream));
		std::string newSource = sourceStream.str();
		if (newSource.empty())
		{
			LOGE("err! shader is empty");
			return false;
		}

		std::vector<std::pair<ShaderUtils::ShaderStageFlagBits, size_t>> stagePositions;
		size_t startOfStage = 0;
		size_t pos = newSource.find('#');

		//Check first #version
		const size_t endOfLine = newSource.find_first_of("\r\n", pos) + 1;
		const std::vector<std::string> tokens = StringUtil::SplitStringAndKeepDelims(newSource.substr(pos, endOfLine - pos));
		if (!(tokens.size() >= 3 && tokens[1] == "version"))
		{
			LOGE("err! Invalid #version encountered or #version is NOT encounted first.");
			return false;
		}
		pos = newSource.find('#', pos + 1);

		while (pos != std::string::npos)
		{
			const size_t endOfLine = newSource.find_first_of("\r\n", pos) + 1;
			std::vector<std::string> tokens = StringUtil::SplitStringAndKeepDelims(newSource.substr(pos, endOfLine - pos));

			size_t index = 1; // Skip #

			if (tokens[index] == ShaderProcessor::MACRO_PRAGMA) // Parse stage. example: #pragma stage : vert
			{
				++index;
				if (tokens[index] == ShaderProcessor::MACRO_PROCESSOR_STAGE)
				{
					++index;// Jump over ':'
					if (tokens[index] != ShaderProcessor::MACRO_PROCESSOR_COLON)
					{
						LOGE("err! Stage pragma is invalid for shader[%s]", m_name.c_str());
						break;
					}

					++index;
					const std::string_view stage = tokens[index];
					if (!(stage == ShaderProcessor::MACRO_STAGE_VERT ||
						stage == ShaderProcessor::MACRO_STAGE_FRAG ||
						stage == ShaderProcessor::MACRO_STAGE_GEO))
					{
						LOGE("err! Invalid stage[%s] for shader[%s]", stage.data(), m_name.c_str());
						break;
					}
					ShaderUtils::ShaderStageFlagBits shaderStage = ShaderUtils::ShaderStageFromString(stage);
					stagePositions.emplace_back(shaderStage, startOfStage);
				}
			}
			else if (tokens[index] == "version")// start pos for next shader stage
			{
				++index;
				startOfStage = pos;
			}

			pos = newSource.find('#', pos + 1);
		}

		if (stagePositions.size() < 2)
		{
			LOGE("err! Could not parse shader[%s], shader stages[%u] are declared invalid", m_name.c_str(), stagePositions.size());
			return false;
		}

		// Get source for each stage
		{
			ShaderUtils::ShaderStageFlagBits stageFlag = stagePositions[0].first;
			std::size_t stagePos = stagePositions[0].second;

			//Get first stage
			const std::string firstStageStr = newSource.substr(0, stagePositions[1].second);
			size_t lineCount = std::count(firstStageStr.begin(), firstStageStr.end(), '\n') + 1;
			stageSource[stageFlag] = firstStageStr;

			//Get stages in the middle
			for (size_t i = 1; i < stagePositions.size() - 1; ++i)
			{
				stageFlag = stagePositions[i].first;
				stagePos = stagePositions[i].second;

				std::string stageStr = newSource.substr(stagePos, stagePositions[i + 1].second - stagePos);
				const size_t secondLinePos = stageStr.find_first_of('\n', 1) + 1;
				stageStr.insert(secondLinePos, fmt::format("#line {}\n", lineCount));
				stageSource[stageFlag] = stageStr;
				lineCount += std::count(stageStr.begin(), stageStr.end(), '\n') + 1;
			}

			//Get last stage
			stageFlag = stagePositions[stagePositions.size() - 1].first;
			stagePos = stagePositions[stagePositions.size() - 1].second;
			std::string lastStageStr = newSource.substr(stagePos);
			const size_t secondLinePos = lastStageStr.find_first_of('\n', 1) + 1;
			lastStageStr.insert(secondLinePos, fmt::format("#line {}\n", lineCount + 1));
			stageSource[stageFlag] = lastStageStr;
		}

		return true;
	}
}