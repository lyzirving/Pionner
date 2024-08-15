#include "GLRenderAPI2.h"

#include "GLHeader.h"
#include "Editor.h"

#include "gfx/renderer/RenderContext.h"
#include "window/Window.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderAPI2"

namespace pio
{
	GLRenderAPI2::GLRenderAPI2(BackendFlags type) : CRenderAPI(), m_type(type)
	{
	}

	bool GLRenderAPI2::setupBackend()
	{
		if (m_bBackend)
		{
			LOGD("render api already initilized");
			return true;
		}
		PIO_ASSERT_RETURN_FALSE(glewInit() == GLEW_OK, "err! glew fail to init");
		GLint major{ 0 }, minor{ 0 };
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		m_bBackend = true;
		LOGD("glew succeed to init, version[%d.%d]", major, minor);
		return true;
	}

	bool GLRenderAPI2::setupUiBackend(void* nativeWindow)
	{
		PIO_ASSERT_RETURN_FALSE(nativeWindow, "invalid native window");
		if (m_bUiBackend)
		{
			LOGD("Ui backend already initilized");
			return true;
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		if (!ImGui::CreateContext())
		{
			LOGE("fail to create ImGui Context");
			return false;
		}
		const char* glslVersion = "#version 430";
		m_bUiBackend = true;

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(nativeWindow), true);
		ImGui_ImplOpenGL3_Init(glslVersion);
		LOGD("init ImGui for glsl[%s]", glslVersion);
		return true;
	}

	void GLRenderAPI2::shutdown()
	{
		if (m_bUiBackend)
		{
			LOGD("destroy ImGui");
			m_bUiBackend = false;
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		m_bBackend = false;
	}

	void GLRenderAPI2::beginFrame(RenderContext& ctx)
	{
		auto& window = ctx.window();

		glViewport(0, 0, window->width(), window->height());
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GLRenderAPI2::endFrame(RenderContext& ctx)
	{
		// Prepare the data for rendering so you can call GetDrawData()
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}