#include "GraphicsContext.h"

#include "platform/opengl/GLContext.h"
#include "gfx/rhi/RenderAPI.h"

namespace pio
{
	Scope<GraphicsContext> GraphicsContext::create(void *window)
	{
		if (RenderAPI::IsOpenGL())
		{
			return CreateScope<GLContext>(window);
		}
		else
		{
			return Scope<GraphicsContext>();
		}
	}
}