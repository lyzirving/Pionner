#include "StringUtil.h"
#include <regex>

#include "StringUtil.h"

namespace pio
{
	namespace StringUtil
	{
		bool Contains(const std::string &src, const std::string &target)
		{
			std::string tSrc{ src }, tTarget{ target };
			std::transform(tSrc.begin(), tSrc.end(), tSrc.begin(), [](const unsigned char c) { return std::tolower(c); });
			std::transform(tTarget.begin(), tTarget.end(), tTarget.begin(), [](const unsigned char c) { return std::tolower(c); });
			return tSrc.find(tTarget) != std::string::npos;
		}

		std::vector<std::string> StringUtil::SplitStringAndKeepDelims(const std::string& str)
		{
			const static std::regex re(R"((^\W|^\w+)|(\w+)|[:()])", std::regex_constants::optimize);

			std::regex_iterator<std::string::const_iterator> rit(str.begin(), str.end(), re);
			std::regex_iterator<std::string::const_iterator> rend;
			std::vector<std::string> result;

			while (rit != rend)
			{
				result.emplace_back(rit->str());
				++rit;
			}
			return result;
		}

		void StringUtil::ToLower(std::string& string)
		{
			std::transform(string.begin(), string.end(), string.begin(), [](const unsigned char c) { return std::tolower(c); });
		}

		std::string StringUtil::ToLower(const std::string& string)
		{
			std::string src = string;
			std::transform(src.begin(), src.end(), src.begin(), [](const unsigned char c) { return std::tolower(c); });
			return src;
		}
	}
}
