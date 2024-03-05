#include "Transform.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Transform"

namespace pio
{
	Transform Transform::operator*(const Transform &rhs)
	{
		Transform ret;
		ret.Position = this->Position + rhs.Position;
		ret.Euler = this->Euler * rhs.Euler;
		ret.Scale = this->Scale * rhs.Scale;
		return ret;
	}
}