#ifndef __PIONNER_PLATFORM_OPENGL_GL_CONTEXT_H__
#define __PIONNER_PLATFORM_OPENGL_GL_CONTEXT_H__

#include "gfx/GraphicsContext.h"

struct GLFWwindow;

namespace pio
{
	class GLContext : public GraphicsContext
	{
	public:
		GLContext(void *window);
		virtual ~GLContext();

		virtual void init() override;
		virtual void swapBuffer() override;

	private:
		GLFWwindow *m_winHandle{ nullptr };
	};
}

#endif