#ifndef __PIONNER_BASE_UTILS_STRING_UTIL_H__
#define __PIONNER_BASE_UTILS_STRING_UTIL_H__

#include "Entry.h"

namespace pio
{
	namespace StringUtil
	{
		/*
		* @brief find out whether the src string contains the target ignoring the case
		* @param src: source string
		* @param target: target string
		* @return true means the src contains target, otherwise, not.
		*/
		bool Contains(const std::string &src, const std::string &target);
		// Keeps delimiters except for spaces, used for shaders
		std::vector<std::string> SplitStringAndKeepDelims(const std::string &str);

		void ToLower(std::string& string);
		std::string ToLower(const std::string& string);

		std::string ReadFileSource(const std::string& path);

		inline std::string DeleteSubstr(const std::string &content, std::size_t start, std::size_t end)
		{
			assert((!content.empty() && (start < end)), "err! invalid input for DeleteSubstr");
			return content.substr(0, start) + content.substr(end);
		}
	}
}

#endif