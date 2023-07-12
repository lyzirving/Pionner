#include "Rhi.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Rhi"

namespace pio
{
	namespace gfx
	{
		Rhi::Rhi()
		{
		}

		Rhi::~Rhi() = default;

		bool Rhi::init()
		{
			if (m_isInit)
				return m_isInit;

			if (glewInit() != GLEW_OK)
			{
				LOG_FATAL("glew init failed");
				return m_isInit;
			}

			GLint major{ 0 }, minor{ 0 };
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);

			LOG_DEBUG("glew succeed to init, gl version[%d.%d]", major, minor);
			m_isInit = true;
			return m_isInit;
		}

		void Rhi::shutdown()
		{
			m_isInit = false;
		}
	}
}