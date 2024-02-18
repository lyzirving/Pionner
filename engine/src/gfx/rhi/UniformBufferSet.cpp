#include "UniformBufferSet.h"

#include "gfx/rhi/RenderAPI.h"

#include "platform/opengl/GLUniformBufferSet.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformBufferSet"

namespace pio
{
	Ref<UniformBufferSet> UniformBufferSet::Create()
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLUniformBufferSet>();
			default:
				return Ref<UniformBufferSet>();
		}
	}
}