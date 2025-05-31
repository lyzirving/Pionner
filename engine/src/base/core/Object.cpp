#include "Object.h"

namespace pio
{
	std::string Object::ToString() const
	{
		std::stringstream ss;
		ss << "type[" << typeid(*this).name() << "]";
		return ss.str();
	}
}