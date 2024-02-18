#include "Base.h"

namespace pio
{
	namespace Base
	{
		std::string GetMemberName(const char *exp)
		{
			std::string ret(exp);
			std::size_t pos = ret.find_last_of(".");
			if (pos != std::string::npos)
				return ret.substr(pos + 1);

			pos = ret.find_last_of("->");
			if (pos != std::string::npos)
				return ret.substr(pos + 1);

			return "invalid expression!";
		}
	}
}