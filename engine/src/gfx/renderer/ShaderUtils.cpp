#include "ShaderUtils.h"

namespace pio
{
	ShaderBuilder& ShaderBuilder::SetPath(const std::string& path)
	{
		m_Path = path;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::SetName(const std::string& name)
	{
		m_Name = name;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::SetMutant(bool val)
	{
		m_bMutant = val;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::AddMacro(const std::string& macro)
	{
		// insert the macro into all stages of the shader
		auto it = m_StageInfo.begin();
		while(it != m_StageInfo.end())
		{
			auto& macros = it->second.Macro;
			auto it2 = macros.find(macro);
			if(it2 == macros.end())
			{
				macros.insert(macro);
				m_bMacroChange = true;
			}
			++it;
		}
		return *this;
	}

	ShaderBuilder& ShaderBuilder::AddMacro(ShaderUtils::ShaderStageFlagBits stage, const std::string& macro)
	{
		auto& info = m_StageInfo[stage];
		auto it = info.Macro.find(macro);
		if(it == info.Macro.end())
		{
			info.Macro.insert(macro);
			m_bMacroChange = true;
		}
		return *this;
	}

	std::string ShaderBuilder::GetFullPath() const
	{
		std::stringstream ss;
		ss << m_Path << Path::FILE_SEPARATOR << m_Name << "." << Path::SHADER_SUFFIX;
		return ss.str(); 
	}
}