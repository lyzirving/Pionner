#include "Rhi.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "global/window/WindowSystem.h"

#include "render/rhi/RhiHeader.h"

#include "gfx/buffer/Texture.h"

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

		bool Rhi::initUiBackend(const std::shared_ptr<WindowSystem> &window)
		{
			if (!window || !window->getWindow())
			{
				LOG_FATAL("window is invalid");
				return false;
			}
			const char *version{ "#version 430" };
			bool ret0 = ImGui_ImplOpenGL3_Init(version);
			bool ret1 = ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);

			if (!ret0 || !ret1)
			{
				LOG_FATAL("fail to init ui render backend, imgui for opengl[%s], imgui glfw[%s]"
						  , ret0 ? "true" : "false", ret1 ? "true" : "false");
				return false;
			}

			LOG_DEBUG("Imgui opengl window[%s] inits success", version);
			return true;
		}

		void Rhi::shutdown()
		{
			m_isInit = false;
		}

		void Rhi::shutdownUiBackend()
		{
			ImGui_ImplGlfw_Shutdown();
			ImGui_ImplOpenGL3_Shutdown();
		}

		bool Rhi::drawTriangleElement(uint32_t indexCnt, DataType type)
		{
			glDrawElements(GL_TRIANGLES, indexCnt, GLHelper::dataType(type), nullptr);
			return GLHelper::checkGLErr("draw call failed for triangle elements");
		}

		void Rhi::unbindTexture(TextureType type)
		{
			glBindTexture(Texture::textureType(type), 0);
		}
	}
}