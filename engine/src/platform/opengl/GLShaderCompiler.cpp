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
		compiler.m_source = StringUtil::ReadFileSource(path);
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
		std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData> stageSource;
		if (!preprocessShader(stageSource))
		{
			LOGE("err! preprocess shader failed!");
			return false;
		}
		preprocessIncluders(stageSource);
		return true;
	}

	bool GLShaderCompiler::preprocessShader(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource)
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
		std::map<ShaderUtils::ShaderStageFlagBits, std::unordered_set<IncludeData>> stageIncluders;		
		size_t startOfStage = 0;
		size_t versionPos = newSource.find('#');
		size_t pos = versionPos;
		ShaderUtils::ShaderStageFlagBits curStage{ ShaderUtils::SHADER_STAGE_ALL };

		//Check first #version
		const size_t versionEnd = newSource.find_first_of("\r\n", versionPos) + 1;
		const std::vector<std::string> tokens = StringUtil::SplitStringAndKeepDelims(newSource.substr(versionPos, versionEnd - versionPos));
		if (!(tokens.size() >= 3 && tokens[1] == ShaderProcessor::MACRO_VERSION))
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
					curStage = ShaderUtils::ShaderStageFromString(stage);
					stagePositions.emplace_back(curStage, startOfStage);
				}
				// Delete current macro
				newSource = StringUtil::DeleteSubStr(newSource, pos, endOfLine);	
			}
			else if (tokens[index] == ShaderProcessor::MACRO_INCLUDE)// start pos for next shader stage
			{		
				IncludeData data;
				data.FilePath = std::string("shader/include/") + tokens[index + 1] + "." + tokens[index + 2];
				data.LineStart = pos;
				data.LineEnd = endOfLine;
				stageIncluders[curStage].insert(data);
			}
			else if (tokens[index] == ShaderProcessor::MACRO_VERSION)// start pos for next shader stage
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
			stageSource[stageFlag].Source = firstStageStr;
			stageSource[stageFlag].Stage = stageFlag;
			stageSource[stageFlag].Includers = stageIncluders[stageFlag];

			//Get stages in the middle
			for (size_t i = 1; i < stagePositions.size() - 1; ++i)
			{
				stageFlag = stagePositions[i].first;
				stagePos = stagePositions[i].second;

				std::string stageStr = newSource.substr(stagePos, stagePositions[i + 1].second - stagePos);
				const size_t secondLinePos = stageStr.find_first_of('\n', 1) + 1;
				stageStr.insert(secondLinePos, fmt::format("#line {}\n", lineCount));
				stageSource[stageFlag].Source = stageStr;
				stageSource[stageFlag].Stage = stageFlag;
				stageSource[stageFlag].Includers = stageIncluders[stageFlag];
				lineCount += std::count(stageStr.begin(), stageStr.end(), '\n') + 1;
			}

			//Get last stage
			stageFlag = stagePositions[stagePositions.size() - 1].first;
			stagePos = stagePositions[stagePositions.size() - 1].second;
			std::string lastStageStr = newSource.substr(stagePos);
			const size_t secondLinePos = lastStageStr.find_first_of('\n', 1) + 1;		
			stageSource[stageFlag].Source = lastStageStr;
			stageSource[stageFlag].Stage = stageFlag;
			stageSource[stageFlag].Includers = stageIncluders[stageFlag];
			lineCount += std::count(lastStageStr.begin(), lastStageStr.end(), '\n') + 1;
		}

		return true;
	}

	bool GLShaderCompiler::preprocessIncluders(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource)
	{
		for (auto& [stage, stageData] : stageSource)
		{	
			std::unordered_map<std::string, bool> expanded{};
			std::string &source = stageData.Source;

			for(auto &data : stageData.Includers)
			{
				//expandIncluder(data, source, expanded);
			}
		}
		return true;
	}	

	void GLShaderCompiler::expandIncluder(const IncludeData &data, std::string &source, std::unordered_map<std::string, bool> &expanded)
	{		
		auto it = expanded.find(data.FilePath);
		if(it != expanded.end())// the include file has already been expanded
			return;

		std::string includerSource = StringUtil::ReadFileSource(data.FilePath);
		if(includerSource.empty())
		{
			LOGE("include file[%s] is invalid", data.FilePath.c_str());
			source = StringUtil::DeleteSubStr(source, data.LineStart, data.LineEnd);	
			return;
		}

		size_t pos = includerSource.find('#');
		while(pos != std::string::npos)
		{
			const size_t endOfLine = includerSource.find_first_of("\r\n", pos) + 1;
			const std::vector<std::string> tokens = StringUtil::SplitStringAndKeepDelims(includerSource.substr(pos, endOfLine - pos));
			if(tokens.size() >= 2 && tokens[1] == ShaderProcessor::MACRO_INCLUDE)
			{
				IncludeData data;
				data.FilePath = std::string("shader/include/") + tokens[2] + "." + tokens[3];
				data.LineStart = pos;  
				data.LineEnd = endOfLine; 
				expandIncluder(data, includerSource, expanded);
			}
			pos = includerSource.find('#', pos + 1);
		}
	}
}