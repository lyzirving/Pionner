#include "StringUtil.h"

namespace pio
{
	namespace StringUtil
	{
		bool contains(const std::string &src, const std::string &target)
		{
			std::string tSrc{ src }, tTarget{ target };
			std::transform(tSrc.begin(), tSrc.end(), tSrc.begin(), std::tolower);
			std::transform(tTarget.begin(), tTarget.end(), tTarget.begin(), std::tolower);
			return tSrc.find(tTarget) != std::string::npos;
		}
	}
}
