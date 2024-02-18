#include "RenderAPI.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderAPI"

namespace pio
{
	RenderAPI::Type RenderAPI::s_type = RenderAPI::Type::OpenGL;

	void RenderAPI::SetType(RenderAPI::Type type)
	{
		LOGW("OpenGL is the only API supported in current implementation.");
	}
}