#include "GLRenderAPI.h"

#include "GLHelper.h"
#include "GLHeader.h"

#include "GlobalSettings.h"
#include "window/Window.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderAPI"

namespace pio
{
	GLRenderAPI::GLRenderAPI(RenderBackendFlags flag) : RenderAPI(flag)
	{
	}

	bool GLRenderAPI::setupBackend()
	{
		if (m_backendSetup)
		{
			LOGD("render api already initilized");
			return true;
		}
		PIO_CHECK_RETURN_FALSE(glewInit() == GLEW_OK, "err! glew fail to init");
		GLint major{ 0 }, minor{ 0 };
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		m_backendSetup = true;
		LOGD("glew succeed to init, version[%d.%d]", major, minor);
		return true;
	}

	bool GLRenderAPI::setupUiBackend(void* nativeWindow)
	{
		PIO_CHECK_RETURN_FALSE(nativeWindow, "invalid native window");
		if (m_uiSetup)
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
		m_uiSetup = true;

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

	void GLRenderAPI::shutdown()
	{
		if (m_uiSetup)
		{
			LOGD("destroy ImGui");
			m_uiSetup = false;
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		m_backendSetup = false;
	}

	void GLRenderAPI::setViewport(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		glViewport(x, y, w, h);
	}

	bool GLRenderAPI::bindUnimBlock(uint32_t program, uint32_t bindingPt, const std::string& blockName)
	{
		if(program == 0)
		{
			LOGE("err! invalid shader program");
			return false;
		}

		if(blockName.empty())
		{
			LOGE("err! empty block name");
			return false;
		}		

		uint32_t index = glGetUniformBlockIndex(program, blockName.c_str());
		if(index == GL_INVALID_INDEX)
		{
			LOGE("err! fail to get block[%s]'s index in program[%u]", blockName.c_str(), program);
			GLHelper::CheckError("fail to get uniform block's index in program");
			return false;
		}

		GLint blockSize{ 0 };
		glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		//LOGD("block[%s] in program[%u]'s size is [%d]", blockName.c_str(), program, blockSize);

		glUniformBlockBinding(program, index, bindingPt);
		return GLHelper::CheckError("fail to bind program[%u]'s uniform block[%s][%u] to binding point[%u]",
									program, blockName.c_str(), index, bindingPt);
	}

	void GLRenderAPI::drawElements(DrawMode mode, uint32_t indexNum, IndexInternalFmt fmt)
	{
		glDrawElements(GLHelper::GetDrawMode(mode), indexNum, GLHelper::GetIndexInternalFormat(fmt), nullptr);
		GLHelper::CheckError("err! drawElements fail");
	}

	void GLRenderAPI::onBeginFrame(RenderContext& context)
	{		
		const auto& vp = context.vp();
		const auto& color = GlobalSettings::RenderConfig.ClearColor;

		glViewport(vp.offsetX(), vp.offsetY(), vp.ratioW(), vp.ratioH());
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GLRenderAPI::onEndFrame(RenderContext& context)
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

	void GLRenderAPI::releaseResource(ResourceGCDelegate&& resource)
	{
		if (resource.Id == 0)
		{
			LOGW("warning! invalid resource id[0] for type[%s]", Rhi::RenderResourceTypeStr(resource.Type));
			return;
		}
		switch (resource.Type)
		{
		case RenderResourceType::UBO:
		case RenderResourceType::VBO:
		case RenderResourceType::EBO:
		{
			LOGD("delete buffer[%u], type[%s]", resource.Id, Rhi::RenderResourceTypeStr(resource.Type));
			glDeleteBuffers(1, &resource.Id);
			break;
		}
		case RenderResourceType::VAO:
		{
			LOGD("delete vertex array[%u], type[%s]", resource.Id, Rhi::RenderResourceTypeStr(resource.Type));
			glDeleteVertexArrays(1, &resource.Id);
			break;
		}
		case RenderResourceType::FBO:
		{
			LOGD("delete fame buffer[%u], type[%s]", resource.Id, Rhi::RenderResourceTypeStr(resource.Type));
			glDeleteFramebuffers(1, &resource.Id);
			break;
		}
		case RenderResourceType::Texture:
		{
			LOGD("delete texture[%u], type[%s]", resource.Id, Rhi::RenderResourceTypeStr(resource.Type));
			glDeleteTextures(1, &resource.Id);
			break;
		}
		case RenderResourceType::RenderBuffer:
		{
			LOGD("delete render buffer[%u], type[%s]", resource.Id, Rhi::RenderResourceTypeStr(resource.Type));
			glDeleteBuffers(1, &resource.Id);
			break;
		}
		case RenderResourceType::Shader:
		{
			LOGD("delete shader[%u], type[%s]", resource.Id, Rhi::RenderResourceTypeStr(resource.Type));
			glDeleteProgram(resource.Id);
			break;
		}
		default:
			LOGE("err! invalid resource type[%s], id[%u]", Rhi::RenderResourceTypeStr(resource.Type), resource.Id);
			break;
		}
	}
}