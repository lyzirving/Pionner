#include "GLShaderCompiler.h"

#include "GLShader.h"
#include "GLHelper.h"
#include "GLHeader.h"

#include <spdlog/fmt/fmt.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLShaderCompiler"

namespace pio
{	
	void PrintTokens(const std::vector<std::string>& tokens)
	{
		if(tokens.empty())
		{
			LOGD("tokens are empty");
			return;
		}
		std::stringstream ss;
		ss << "tokens: ";
		for(size_t i = 0; i < tokens.size(); i++)
		{
			ss << "[" << i << "] = " << tokens[i].c_str();
			if(i < tokens.size() - 1)
			{
				ss << ", ";
			}
		}
		ss << "\0\n";
		LOGD("%s", ss.str().c_str());
	}

	GLShaderCompiler::GLShaderCompiler() : ShaderCompiler()
	{
	}

	void GLShaderCompiler::ParseMetadata()
	{
		size_t found = m_Path.find_last_of("/");
		m_Name = (found != std::string::npos) ? m_Path.substr(found + 1) : m_Path;
		found = m_Name.find_last_of('.');
		std::string extention = (found != std::string::npos) ? m_Name.substr(found) : m_Name;
		m_Lang = ShaderUtils::LanguageFromExtension(extention);
	}

	bool GLShaderCompiler::Preprocess(ShaderBuilder& builder)
	{
		switch (m_Lang)
		{
		case ShaderUtils::SourceLang::GLSL:
			return PreprocessGLSL(builder);
		default:
			break;
		}
		return false;
	}

	Ref<Shader> GLShaderCompiler::CreateShader(const Ref<RenderContext>& context, const ShaderBuilder& builder)
	{
		if(m_StageSource.empty())
		{
			LOGE("stage source is empty");
			return Ref<Shader>();
		}
		Ref<Shader> shader;
		if(builder.IsMutant())
		{
			shader = CreateRef<GLShaderMutant>(context, m_Name, m_StageSource);
		}
		else
		{
			shader = CreateRef<GLShader>(context, m_Name, m_StageSource);		
		}
		return shader;
	}

	bool GLShaderCompiler::PreprocessGLSL(ShaderBuilder& builder)
	{
		if (!PreprocessStages(m_StageSource, builder))
		{
			LOGE("err! Preprocess shader failed!");
			return false;
		}
		PreprocessIncluders(m_StageSource);
		return true;
	}

	bool GLShaderCompiler::PreprocessStages(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource, ShaderBuilder& builder)
	{
		std::stringstream sourceStream;
		// clear comments in shader
		ShaderProcessor::CopyWithoutComments(m_Source.begin(), m_Source.end(), std::ostream_iterator<char>(sourceStream));
		std::string newSource = sourceStream.str();
		if (newSource.empty())
		{
			LOGE("err! shader is empty");
			return false;
		}

		std::vector<std::pair<ShaderUtils::ShaderStageFlagBits, size_t>> stagePositions;
		std::map<ShaderUtils::ShaderStageFlagBits, std::vector<IncludeData>> stageIncluders;
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
				if (tokens[index] == ShaderProcessor::MACRO_PRAGMA_STAGE)
				{
					++index;// Jump over ':'
					if (tokens[index] != ShaderProcessor::MACRO_PRAGMA_COLON)
					{
						LOGE("err! Stage pragma is invalid for shader[%s]", m_Name.c_str());
						break;
					}

					++index;
					const std::string_view stage = tokens[index];
					if (!(stage == ShaderProcessor::MACRO_STAGE_VERT ||
						stage == ShaderProcessor::MACRO_STAGE_FRAG ||
						stage == ShaderProcessor::MACRO_STAGE_GEO))
					{
						LOGE("err! Invalid stage[%s] for shader[%s]", stage.data(), m_Name.c_str());
						break;
					}
					curStage = ShaderUtils::ShaderStageFromString(stage);
					stagePositions.emplace_back(curStage, startOfStage);
					// Delete current macro
					newSource = StringUtil::DeleteSubstr(newSource, pos, endOfLine);
					auto& stageInfo = builder.GetStageInfo();
					auto it = stageInfo.find(curStage);
					// Insert stage macro if need
					if (it != stageInfo.end() && !it->second.Macro.empty())
					{
						//the cursor is on #precision line, now find its end
						size_t cursor = newSource.find_first_of("\r\n", pos);
						auto& stageMacros = it->second.Macro;
						auto macroIt = stageMacros.begin();
						while (macroIt != stageMacros.end())
						{
							// current cursor is the end of current line
							// move it to new line
							++cursor;
							std::string content("#define ");							
							content.append(*macroIt).
								    append("\r\n");
							newSource.insert(cursor, content);
							//find end of current insertion
							cursor = newSource.find_first_of("\r\n", cursor);
							++macroIt; 
						}						
						pos = cursor;
					}
					else
					{
						if(it == stageInfo.end())
						{
							ShaderStageInfo info;
							stageInfo.insert({curStage, info});
						}
						// Back to last line end
						pos = (pos == 0) ? 0 : pos - 1;
					}					
				}
				else
				{
					LOGE("Invalid param[%s] in macro pragma when parse stage", tokens[index].c_str());
					assert(0);
				}
			}
			else if (tokens[index] == ShaderProcessor::MACRO_INCLUDE)// start pos for next shader stage
			{
				IncludeData data;
				std::string fullPath = ShaderUtils::ShaderIncludePath();
				size_t tokenSize = tokens.size();
				for (size_t i = index + 1; i < (tokenSize - 1); ++i)
				{
					fullPath.append(tokens[i]).
						     append(i == (tokenSize - 2) ? "." : "/");
				}
				fullPath.append(tokens[tokenSize - 1]);
				data.FilePath = fullPath;
				data.LineStart = pos - startOfStage;
				data.LineEnd = endOfLine - startOfStage;
				stageIncluders[curStage].push_back(std::move(data));
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
			LOGE("err! Could not parse shader[%s], shader stages[%u] are declared invalid", m_Name.c_str(), stagePositions.size());
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

	bool GLShaderCompiler::PreprocessIncluders(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource)
	{
		for (auto& [stage, stageData] : stageSource)
		{
			std::unordered_map<std::string, bool> expanded{};
			std::string& source = stageData.Source;
			ShaderUtils::StageData rawData = stageData;
			bool dealNext{ false };
			int32_t diff{ 0 };

			for (int32_t i = 0; i < stageData.Includers.size(); i++)
			{
				dealNext = i < stageData.Includers.size() - 1;
				auto& cur = stageData.Includers[i];
				if(dealNext)
				{
					diff = rawData.Includers[i + 1].LineStart - rawData.Includers[i].LineEnd;
					diff = std::max(0, diff);
				}
				ExpandIncluder(cur, source, expanded);
				if (dealNext)
				{
					auto &next = stageData.Includers[i + 1];
					size_t len = next.LineEnd - next.LineStart;
					next.LineStart = cur.LineEnd + diff - 1;
					next.LineEnd = next.LineStart + len;
				}
			}
		}
		return true;
	}

	void GLShaderCompiler::ExpandIncluder(IncludeData& data, std::string& source, std::unordered_map<std::string, bool> &expanded)
	{
		auto it = expanded.find(data.FilePath);
		if (it != expanded.end())// the include file has already been expanded
		{
			source = StringUtil::DeleteSubstr(source, data.LineStart, data.LineEnd);
			return;
		}

		std::string includerSource = StringUtil::ReadFileSource(data.FilePath);
		if (includerSource.empty())
		{
			LOGE("include file[%s] is invalid", data.FilePath.c_str());
			source = StringUtil::DeleteSubstr(source, data.LineStart, data.LineEnd);
			return;
		}

		std::stringstream sourceStream;
		// clear comments in includer
		ShaderProcessor::CopyWithoutComments(includerSource.begin(), includerSource.end(), std::ostream_iterator<char>(sourceStream));
		includerSource = sourceStream.str();
		if(includerSource.empty())
		{
			LOGE("err! includer[%s] is invalid after remove comments", data.FilePath.c_str());
			return;
		}

		size_t pos = includerSource.find('#');
		while (pos != std::string::npos)
		{
			size_t endOfLine = includerSource.find_first_of("\r\n", pos);
			endOfLine = (endOfLine == std::string::npos) ? includerSource.size() : (endOfLine + 1);
			const std::vector<std::string> tokens = StringUtil::SplitStringAndKeepDelims(includerSource.substr(pos, endOfLine - pos));
			//PrintTokens(tokens);
			bool deleteSource{ false };
			if (tokens.size() >= 2)
			{
				if (tokens[1] == ShaderProcessor::MACRO_INCLUDE)
				{
					// tokens[0] = #, tokens[1] = include, tokens[2] = file name, tokens[3] = file format 
					assert(tokens[tokens.size() - 1] == "glslh", "includer format err, %s should be glslh", data.FilePath.c_str());
					IncludeData newData;
					std::string fullPath = ShaderUtils::ShaderIncludePath();
					size_t tokenSize = tokens.size();
					for (size_t i = 2; i < (tokenSize - 1); ++i)
					{
						fullPath.append(tokens[i]).
							append(i == (tokenSize - 2) ? "." : "/");
					}
					fullPath.append(tokens[tokenSize - 1]);
					newData.FilePath = fullPath;
					newData.LineStart = pos;
					newData.LineEnd = endOfLine;
					ExpandIncluder(newData, includerSource, expanded);
				}
				else if (tokens[1] == ShaderProcessor::MACRO_PRAGMA && tokens[2] == ShaderProcessor::MACRO_PRAGMA_ONECE)
				{
					deleteSource = true;
					expanded[data.FilePath] = true;
				}
				else if(tokens[1] == ShaderProcessor::MACRO_IFDEF || tokens[1] == ShaderProcessor::MACRO_ELSE ||
						tokens[1] == ShaderProcessor::MACRO_ENDIF)
				{
					pos += 1;
				}
			}	
			else
			{
				deleteSource = true;
			}
			if (deleteSource)
			{
				includerSource = StringUtil::DeleteSubstr(includerSource, pos, endOfLine);
			}
			pos = includerSource.find('#', pos);
		}
		source = StringUtil::DeleteSubstr(source, data.LineStart, data.LineEnd);
		source.insert(source.begin() + data.LineStart, includerSource.begin(), includerSource.end());
		source.insert(source.begin() + data.LineStart + includerSource.size(), '\n');
		data.LineEnd = data.LineStart + includerSource.size() + 2;
	}
}