#include <iostream>
#include <fstream>
#include <sstream>

#include "ShaderUtils.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderUtils"

namespace pio
{
	std::string ShaderUtils::ReadShaderSource(const std::string& path)
	{
		std::ifstream file;
		std::stringstream ss;
		std::string source;

		file.open(path);
		if (file.is_open())
		{
			ss << file.rdbuf();
			file.close();
			source = ss.str();
		}
		else
		{
			LOGE("fail to open shader file[%s]", path.c_str());
		}
		return source;
	}
}