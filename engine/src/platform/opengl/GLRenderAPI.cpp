#include "GLRenderAPI.h"
#include "GLHelper.h"
#include "GLState.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "Application.h"
#include "asset/AssetsManager.h"
#include "core/utils/Profiler.h"

#include "scene/Skybox.h"

#include "gfx/struct/MaterialAsset.h"
#include "gfx/struct/ShaderLibrary.h"
#include "gfx/struct/Geometry2D.h"
#include "gfx/struct/MeshBuilder.h"
#include "gfx/struct/Camera.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"
#include "gfx/rhi/RenderBuffer.h"
#include "gfx/rhi/UniformBufferSet.h"
#include "gfx/rhi/Material.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/RenderPass.h"

#include "gfx/renderer/RenderData.h"
#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderAPI"

namespace pio
{
	GLRenderAPI::GLRenderAPI() : RenderAPI()
	{
	}

	GLRenderAPI::~GLRenderAPI() = default;

	void GLRenderAPI::init()
	{
		initOpenGL();
		initImGui();
	}

	void GLRenderAPI::shutdown()
	{
		destroyImGui();
	}

	void GLRenderAPI::beginFrame()
	{
		m_globalState = m_defaultState;

		commitViewport(0, 0, Application::MainWindow()->getWidth(), Application::MainWindow()->getHeight());
		
		GLState::SetClear(m_globalState.Clear);
		submitRenderState(m_globalState);	

		// TODO: global render attributes
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	void GLRenderAPI::endFrame()
	{
	}

	void GLRenderAPI::beginUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		{
			//BUG FIX: ImGui::NewFrame() will call FindLatestInputEvent(), which will
			//         query the size of this event queue: &g.InputEventsQueue[n];
			//         Since poll event is called in Application thread, while this method
			//         is called in renderer thread, so at some point it will cause crash.
			//         So we use a lock to guarantee thread-safety.
			//         Referencing to https://github.com/ocornut/imgui/issues/6895
			std::lock_guard<std::mutex> lk{ Application::Get()->EventMutex };
			ImGui::NewFrame();
		}
	}

	void GLRenderAPI::endUI()
	{
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO &io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void GLRenderAPI::commitViewport(const Viewport &viewport)
	{
		m_viewport = viewport;
		glViewport(m_viewport.X, m_viewport.Y, m_viewport.Width, m_viewport.Height);
	}

	void GLRenderAPI::commitViewport(int32_t x, int32_t y, int32_t width, int32_t height)
	{
		m_viewport.X = x;
		m_viewport.Y = y;
		m_viewport.Width = width;
		m_viewport.Height = height;
		glViewport(m_viewport.X, m_viewport.Y, m_viewport.Width, m_viewport.Height);
	}

	void GLRenderAPI::framebufferBlockTransfer(uint32_t readBuffer, uint32_t dstBuffer, 
											   glm::ivec2 srcPos, glm::ivec2 srcSize, 
											   glm::ivec2 dstPos, glm::ivec2 dstSize,
											   FrameBufferBlockBit flags, TextureFilterMag filter)
	{
		GLbitfield fields{ GL_NONE };
		if (flags & FB_ColorBuffer_Bit) { fields |= GL_COLOR_BUFFER_BIT; }
		if (flags & FB_DepthBuffer_Bit) 
		{ 
			fields |= GL_DEPTH_BUFFER_BIT; 
			// filter must be nearest when transferring depth buffer
			filter = TextureFilterMag::Nearest;
		}
		if (flags & FB_StencilBuffer_Bit) 
		{ 
			fields |= GL_STENCIL_BUFFER_BIT;
			// filter must be nearest when transferring stencil buffer
			filter = TextureFilterMag::Nearest;
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, readBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstBuffer);
		glBlitFramebuffer(srcPos.x, srcPos.y, srcSize.x, srcSize.y, 
						  dstPos.x, dstPos.y, dstSize.x, dstSize.y, 
						  fields, GLHelper::GetGLTextureMagFilter(filter));
		GLHelper::CheckError("fail to copy block from fbo[%u] to fbo[%u]", readBuffer, dstBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, dstBuffer);
	}

	void GLRenderAPI::renderSubmesh(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		switch (state.Mode)
		{			
			case RenderMode::MaterialPreview:
			{
				drawMatPreview(meshHandle, submeshIndex, materialTable, uniformBufferSet, modelMat, state, DrawMode::Triangles);
				break;
			}
			case RenderMode::Wireframe:
			{
				drawMatPreview(meshHandle, submeshIndex, materialTable, uniformBufferSet, modelMat, state, DrawMode::Line_Strip);
				break;
			}
			case RenderMode::PBR:				
			default:
				LOGE("Err, RenderMode[%u] has not been implemented yet", state.Mode);
				std::abort();
				break;
		}
	}

	void GLRenderAPI::renderDistantLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		PIO_ASSERT_RETURN(shadowPass.use_count() != 0, "renderDistantLightShadow: invalid shadow pass");

		Ref<MeshBase> meshBase = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(meshBase.use_count() != 0, "renderDistantLightShadow: fail to get mesh from[%u]", (uint32_t)meshHandle);

		auto shader = ShaderLibrary::Get()->find(ShaderProgram::DistLightShadowMap);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "DistLightShadowMap shader is invalid");

		Ref<MeshSource> meshSource = meshBase->getMeshSource();

		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];
		Ref<UniformBuffer> boneTransformUB = meshSource->getBoneTransformUB();
		Ref<UniformBuffer> sdUB = uniformBufferSet->get(PIO_UINT(UBBindings::DistantLightShadowData));

		compareAndUpdateRenderState(m_globalState, state);

		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

		shader->bind(true);

		UniformBuffer::Binding(shader, "SdMatrices", sdUB->getBinding());
		sdUB->bind();

		Ref<BufferTexture> gpuAnimBuffer;
		if (boneTransformUB)
		{
			const BoneTransformUD &boneUD = meshSource->getBoneTransformUD();
			shader->setBool("u_bGpuAnimated", boneUD.GPUAnimated);
			if (boneUD.GPUAnimated)
			{
				shader->setInt("u_boneNum", boneUD.GPUTransform.BoneNum);
				shader->setInt("u_frameIndex", boneUD.GPUTransform.CurrentFrame);
				gpuAnimBuffer = boneUD.GPUTransform.BufferTexture;
				gpuAnimBuffer->active(PIO_UINT(TextureSampler::Slot0));
				gpuAnimBuffer->bind();
				shader->setInt("u_gpuAnimBuffer", PIO_INT(TextureSampler::Slot0));
			}
			else
			{
				UniformBuffer::Binding(shader, "BoneMatrices", boneTransformUB->getBinding());
				boneTransformUB->setData(boneUD.Block.getBuffer()->as<void *>(), boneUD.Block.getByteUsed());
				boneTransformUB->bind();
			}
		}

		shader->setMat4("u_modelMat", modelMat);
		shader->setBool("u_playAnimation", boneTransformUB.get());

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderDistantLightShadow fail!!");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		if (boneTransformUB) boneTransformUB->unbind();
		if (gpuAnimBuffer) gpuAnimBuffer->unbind();
		sdUB->unbind();

		shader->bind(false);
	}

	void GLRenderAPI::renderPointLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		PIO_ASSERT_RETURN(shadowPass.use_count() != 0, "renderPointLightShadow: invalid shadow pass");

		Ref<MeshBase> meshBase = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(meshBase.use_count() != 0, "renderPointLightShadow: fail to get mesh from[%u]", (uint32_t)meshHandle);	

		auto shader = ShaderLibrary::Get()->find(ShaderProgram::PtLightShadowMap);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "PtLightShadowMap shader is invalid");

		Ref<MeshSource> meshSource = meshBase->getMeshSource();

		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];
		Ref<UniformBuffer> boneTransformUB = meshSource->getBoneTransformUB();

		compareAndUpdateRenderState(m_globalState, state);

		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

		shader->bind(true);

		auto ptShadowDataUB = uniformBufferSet->get(PIO_UINT(UBBindings::PointLightShadowData));
		UniformBuffer::Binding(shader, "PointLightShadowData", ptShadowDataUB->getBinding());
		ptShadowDataUB->bind();

		Ref<BufferTexture> gpuAnimBuffer;
		if (boneTransformUB)
		{
			const BoneTransformUD &boneUD = meshSource->getBoneTransformUD();
			shader->setBool("u_bGpuAnimated", boneUD.GPUAnimated);
			if (boneUD.GPUAnimated)
			{
				shader->setInt("u_boneNum", boneUD.GPUTransform.BoneNum);
				shader->setInt("u_frameIndex", boneUD.GPUTransform.CurrentFrame);
				gpuAnimBuffer = boneUD.GPUTransform.BufferTexture;
				gpuAnimBuffer->active(PIO_UINT(TextureSampler::Slot0));
				gpuAnimBuffer->bind();
				shader->setInt("u_gpuAnimBuffer", PIO_INT(TextureSampler::Slot0));
			}
			else
			{
				UniformBuffer::Binding(shader, "BoneMatrices", boneTransformUB->getBinding());
				boneTransformUB->setData(boneUD.Block.getBuffer()->as<void *>(), boneUD.Block.getByteUsed());
				boneTransformUB->bind();
			}
		}

		shader->setMat4("u_modelMat", modelMat);
		shader->setBool("u_playAnimation", boneTransformUB.get());

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderPointLightShadow fail!!");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		ptShadowDataUB->unbind();
		if (boneTransformUB) boneTransformUB->unbind();
		if (gpuAnimBuffer) gpuAnimBuffer->unbind();
		shader->bind(false);
	}	

	void GLRenderAPI::renderSubmesh_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		switch (state.Mode)
		{
			case RenderMode::PBR:
			case RenderMode::MaterialPreview:
			{
				drawSubmesh_deferred(meshHandle, submeshIndex, materialTable, uniformBufferSet, modelMat, state);
				break;
			}
			case RenderMode::Wireframe:
			default:
				LOGE("render mode[%u] has not been implemented", state.Mode);
				abort();
				break;
		}
	}

	void GLRenderAPI::renderLightEffect_Deffered(AssetHandle& meshHandle, Ref<Skybox>& skybox, Ref<UniformBufferSet>& ubs, const Ref<RenderPass>& GBufferPass, const Ref<RenderPass>& shadowPass, const Ref<RenderPass>& ptShadowPass, const RenderState& state)
	{
		Ref<QuadMesh> quadMesh = AssetsManager::GetRuntimeAsset<QuadMesh>(meshHandle);
		PIO_ASSERT_RETURN(quadMesh.use_count() != 0, "renderLightEffect_Deffered: Quad Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderProgram::LightingPass);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "LightingPass shader is invalid");

		PIO_ASSERT_RETURN(GBufferPass.use_count() != 0, "GBuffer pass is invalid");
		Ref<FrameBuffer> gBuffer = GBufferPass->getFramebuffer();

		PIO_ASSERT_RETURN(shadowPass.use_count() != 0, "shadow pass is invalid");
		Ref<Texture> shadowBuffer = shadowPass->getFramebuffer()->getDepthBuffer();

		PIO_ASSERT_RETURN(skybox.use_count() != 0, "skybox is invalid");
		// TODO: add case when IBL is invalid
		Ref<CubeTexture> diffuseMap = skybox->getDiffuseMap();
		Ref<CubeTexture> prefilterMap = skybox->getPrefilterMap();
		Ref<Texture2D> brdfLUT = skybox->getBrdfLUT();

		auto cameraUB = ubs->get(PIO_UINT(UBBindings::Camera));
		auto dirLightUB = ubs->get(PIO_UINT(UBBindings::DistantLight));
		auto dirLightSdUB = ubs->get(PIO_UINT(UBBindings::DistantLightShadowData));
		auto pointLightDataUB = ubs->get(PIO_UINT(UBBindings::PointLightData));

		compareAndUpdateRenderState(m_globalState, state);

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		UniformBuffer::Binding(shader, "DirectionalLight", dirLightUB->getBinding());
		UniformBuffer::Binding(shader, "SdMatrices", dirLightSdUB->getBinding());
		UniformBuffer::Binding(shader, "PointLightData", pointLightDataUB->getBinding());

		cameraUB->bind();
		dirLightUB->bind();
		dirLightSdUB->bind();
		pointLightDataUB->bind();

		auto positionBuf = gBuffer->getColorBuffer(ColorAttachment::Attach0);
		auto normalBuf = gBuffer->getColorBuffer(ColorAttachment::Attach1);
		auto albedoBuf = gBuffer->getColorBuffer(ColorAttachment::Attach2);
		auto matBuf = gBuffer->getColorBuffer(ColorAttachment::Attach3);
		auto emissionBuf = gBuffer->getColorBuffer(ColorAttachment::Attach4);

		shader->setTextureSampler("u_GPosition", TextureSampler::Slot0);
		positionBuf->active(PIO_UINT(TextureSampler::Slot0));
		positionBuf->bind();

		shader->setTextureSampler("u_GNormal", TextureSampler::Slot1);
		normalBuf->active(PIO_UINT(TextureSampler::Slot1));
		normalBuf->bind();

		shader->setTextureSampler("u_GAlbedoAlpha", TextureSampler::Slot2);
		albedoBuf->active(PIO_UINT(TextureSampler::Slot2));
		albedoBuf->bind();

		shader->setTextureSampler("u_GMaterial", TextureSampler::Slot3);
		matBuf->active(PIO_UINT(TextureSampler::Slot3));
		matBuf->bind();

		shader->setTextureSampler("u_GEmission", TextureSampler::Slot4);
		emissionBuf->active(PIO_UINT(TextureSampler::Slot4));
		emissionBuf->bind();

		shader->setTextureSampler("u_shadowMap", TextureSampler::Slot5);
		shadowBuffer->active(PIO_UINT(TextureSampler::Slot5)); 
		shadowBuffer->bind();

		shader->setTextureSampler("u_irradianceMap", TextureSampler::Slot6);
		diffuseMap->active(PIO_UINT(TextureSampler::Slot6));
		diffuseMap->bind();

		shader->setTextureSampler("u_prefilterMap", TextureSampler::Slot7);
		prefilterMap->active(PIO_UINT(TextureSampler::Slot7));
		prefilterMap->bind();

		shader->setTextureSampler("u_brdfLUT", TextureSampler::Slot8);
		brdfLUT->active(PIO_UINT(TextureSampler::Slot8));
		brdfLUT->bind();

		shader->setVec4("u_bgColor", Renderer::GetConfig().ClearColor);
		shader->setFloat("u_envMapIntensity", skybox->getIntensity());

		Ref<Texture> ptShadowBuffer;
		if (ptShadowPass)
		{
			TextureSampler ptShadowSlot;
			ptShadowBuffer = ptShadowPass->getSpecification().FrameBuffer->getDepthBuffer();
			if (ptShadowBuffer && shader->getAvailableSampler(ptShadowSlot))
			{
				shader->setTextureSampler("u_pointLightDepthBuffer", ptShadowSlot);
				ptShadowBuffer->active(PIO_UINT(ptShadowSlot));
				ptShadowBuffer->bind();
			}
		}

		quadMesh->VertexArray->bind();
		quadMesh->IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, quadMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderLightEffect_Deffered fail!!");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		quadMesh->IndexBuffer->unbind();
		quadMesh->VertexArray->unbind();

		cameraUB->unbind();
		dirLightUB->unbind();
		dirLightSdUB->unbind();
		pointLightDataUB->unbind();

		positionBuf->unbind();
		normalBuf->unbind();
		albedoBuf->unbind();
		matBuf->unbind();
		emissionBuf->unbind();

		diffuseMap->unbind();
		prefilterMap->unbind();
		brdfLUT->unbind();
		shadowBuffer->unbind();
		if (ptShadowBuffer) { ptShadowBuffer->unbind(); }

		shader->bind(false);
	}

	void GLRenderAPI::renderLineSegment(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &trans, const RenderState &state)
	{
		Ref<LineSegment> lineMesh = AssetsManager::GetRuntimeAsset<LineSegment>(meshHandle);
		PIO_ASSERT_RETURN(lineMesh.use_count() != 0, "renderLineSegment: LineSegment is invalid");
		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::LineSegment);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "renderLineSegment: LineSegment shader is invalid");

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));

		compareAndUpdateRenderState(m_globalState, state);

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		cameraUB->bind();

		shader->setMat4("u_modelMat", trans);
		shader->setVec4("u_color", lineMesh->Color);

		lineMesh->VertexArray->bind();
		lineMesh->IndexBuffer->bind();

		glDrawElements(GL_LINES, lineMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderLineSegment fail!!");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		lineMesh->IndexBuffer->unbind();
		lineMesh->VertexArray->unbind();

		cameraUB->unbind();

		shader->bind(false);
	}

	void GLRenderAPI::renderLine(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &trans, const RenderState &state)
	{
		Ref<LineMesh> lineMesh = AssetsManager::GetRuntimeAsset<LineMesh>(meshHandle);
		PIO_ASSERT_RETURN(lineMesh.use_count() != 0, "Line Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::Color_Line);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "ColorLine shader is invalid");

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));

		compareAndUpdateRenderState(m_globalState, state);

		shader->bind(true);
	
		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		cameraUB->bind();
		shader->setMat4("u_modelMat", trans);

		lineMesh->VertexArray->bind();
		lineMesh->IndexBuffer->bind();

		glDrawElements(GL_LINES, lineMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderLine fail!!");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		lineMesh->IndexBuffer->unbind();
		lineMesh->VertexArray->unbind();

		cameraUB->unbind();

		shader->bind(false);
	}

	void GLRenderAPI::renderTextureQuad2D(AssetHandle &meshHandle, Ref<Texture2D> &texture, const RenderState &state)
	{
		Ref<QuadMesh> quadMesh = AssetsManager::GetRuntimeAsset<QuadMesh>(meshHandle);
		PIO_ASSERT_RETURN(quadMesh.use_count() != 0, "renderTextureQuad2D: Quad Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::TextureQuad);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "TextureQuad shader is invalid");

		PIO_ASSERT_RETURN(texture.use_count() != 0, "renderTextureQuad2D: texture is invalid");

		compareAndUpdateRenderState(m_globalState, state);

		shader->bind(true);

		texture->active(PIO_UINT(TextureSampler::Slot0));
		texture->bind();

		shader->setTextureSampler("u_quadTexture", TextureSampler::Slot0);

		quadMesh->VertexArray->bind();
		quadMesh->IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, quadMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderTextureQuad2D fail!!");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		quadMesh->IndexBuffer->unbind();
		quadMesh->VertexArray->unbind();

		texture->unbind();

		shader->bind(false);
	}

	void GLRenderAPI::renderSprites(const std::vector<SpriteCommand> &cmds)
	{		
		PIO_ASSERT_RETURN(!cmds.empty(), "renderSprites: empty cmds");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderProgram::Sprite);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "renderSprites: Sprite shader is invalid");

		// Reduce shader switch, which is the most consuming
		shader->bind(true);
		
		for(auto &item : cmds)
		{
			Ref<QuadMesh> mesh = AssetsManager::GetRuntimeAsset<QuadMesh>(item.QuadMesh);			
			Ref<Texture2D> texture = AssetsManager::GetRuntimeAsset<Texture2D>(item.Texture);
			PIO_ASSERT_CONTINUE(mesh.use_count() != 0, "renderSprites: Quad mesh is invalid");
			PIO_ASSERT_CONTINUE(texture.use_count() != 0, "renderSprites: texture is invalid");

			compareAndUpdateRenderState(m_globalState, item.State);
			texture->active(PIO_UINT(TextureSampler::Slot0));
			texture->bind();

			shader->setTextureSampler("u_texture", TextureSampler::Slot0);
			shader->setBool("u_bSRGB", texture->SRGB());
			shader->setBool("u_bGammaCorrect", item.bGammaCorrect);

			mesh->VertexArray->bind();
			mesh->IndexBuffer->bind();

			glDrawElements(GL_TRIANGLES, mesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			GLHelper::CheckError("renderSprites fail!!");

			mesh->IndexBuffer->unbind();
			mesh->VertexArray->unbind();

			texture->unbind();
		}

	#ifdef PIO_PROFILER_ON
		glFinish();
	#endif // PIO_PROFILER_ON

		shader->bind(false);
	}

	void GLRenderAPI::renderSkybox(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, Ref<CubeTexture> &cubeTexture, const RenderState &state)
	{
		Ref<MeshBase> cubeMesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(cubeMesh.use_count() != 0, "renderSkybox: Cube Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderProgram::SkyBox);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "SkyBox shader is invalid");

		PIO_ASSERT_RETURN(cubeTexture.use_count() != 0, "renderSkybox: cubeTexture is invalid");

		Ref<MeshSource> meshSource = cubeMesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];
		Ref<UniformBuffer> cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));

		compareAndUpdateRenderState(m_globalState, state);

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		cameraUB->bind();

		cubeTexture->active(PIO_UINT(TextureSampler::Slot0));
		cubeTexture->bind();

		shader->setTextureSampler("u_envMap", TextureSampler::Slot0);

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderSkybox fail!!");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		cameraUB->unbind();
		cubeTexture->unbind();
		shader->bind(false);
	}

	void GLRenderAPI::renderHDRToEnvMap(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment envMapAttachment, Ref<Texture2D> &HDRTexture, Ref<FrameBuffer> &fbo)
	{
		Ref<MeshBase> cubeMesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(cubeMesh.use_count() != 0, "renderHDRToEnvMap: Cube Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderProgram::IBL_EquirecToCube);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "IBL_EquirecToCube shader is invalid");

		PIO_ASSERT_RETURN(HDRTexture.use_count() != 0, "renderHDRToEnvMap: HDRTexture is invalid");
		PIO_ASSERT_RETURN(fbo.use_count() != 0, "renderHDRToEnvMap: fbo is invalid");

		Ref<MeshSource> meshSource = cubeMesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		shader->bind(true);

		shader->setMat4("u_prjMat", prjMat);

		HDRTexture->active(PIO_UINT(TextureSampler::Slot0));
		HDRTexture->bind();
		shader->setTextureSampler("u_equirectangularMap", TextureSampler::Slot0);

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		for (uint8_t i = 0; i < LightDir_Num; i++)
		{			
			shader->setMat4("u_viewMat", viewMat[i]);
			fbo->bindTarget(envMapAttachment, LightDir(i));
			GLState::SetClear(Clear::Common());			
			glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			if (GLHelper::CheckError("renderHDRToEnvMap fail at light dir[%u]!!", i))
				LOGD("succeed to render HDR to cube face[%s]", LightDirStr(LightDir(i)));
		}	
#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		HDRTexture->unbind();
		shader->bind(false);
	}

	void GLRenderAPI::renderDiffuseCnvl(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment diffuseMapAttachment, Ref<CubeTexture> &envMap, Ref<FrameBuffer> &fbo)
	{
		Ref<MeshBase> cubeMesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(cubeMesh.use_count() != 0, "renderDiffuseCnvl: Cube Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderProgram::IBL_DiffuseCnvl);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "IBL_DiffuseCnvl shader is invalid");

		PIO_ASSERT_RETURN(envMap.use_count() != 0, "renderDiffuseCnvl: envMap is invalid");
		PIO_ASSERT_RETURN(fbo.use_count() != 0, "renderDiffuseCnvl: fbo is invalid");

		Ref<Texture2D> diffuseMap = fbo->getColorBuffer(diffuseMapAttachment);
		PIO_ASSERT_RETURN(diffuseMap.use_count() != 0, "renderDiffuseCnvl: prefilter map is invalid");
		Ref<Texture> depthBuf = fbo->getDepthBuffer();
		PIO_ASSERT_RETURN(depthBuf.use_count() != 0, "renderDiffuseCnvl: depth buffer is invalid");
		RenderBuffer *renderBuf = depthBuf->as<RenderBuffer>();
		PIO_ASSERT_RETURN(renderBuf != nullptr, "renderDiffuseCnvl: render buffer is invalid");
		const uint32_t width  = diffuseMap->getWidth();
		const uint32_t height = diffuseMap->getHeight();

		Ref<MeshSource> meshSource = cubeMesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		saveViewport(m_viewport);

		commitViewport(0, 0, width, height);
		renderBuf->bind(width, height);

		shader->bind(true);

		shader->setMat4("u_prjMat", prjMat);

		envMap->active(PIO_UINT(TextureSampler::Slot0));
		envMap->bind();
		shader->setTextureSampler("u_envMap", TextureSampler::Slot0);		

		for (uint8_t i = 0; i < LightDir_Num; i++)
		{
			shader->setMat4("u_viewMat", viewMat[i]);
			fbo->bindTarget(diffuseMapAttachment, LightDir(i));
			GLState::SetClear(Clear::Common());

			submesh.VertexArray->bind();
			submesh.IndexBuffer->bind();

			glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			if (GLHelper::CheckError("renderDiffuseCnvl fail at light dir[%u]!!", i))
				LOGD("succeed to render diffuse convolution to cube face[%s]", LightDirStr(LightDir(i)));
#ifdef PIO_PROFILER_ON
			glFinish();
#endif // PIO_PROFILER_ON

			submesh.IndexBuffer->unbind();
			submesh.VertexArray->unbind();
		}
	
		envMap->unbind();
		shader->bind(false);

		m_viewport = restoreViewport();
	}

	void GLRenderAPI::renderBrdfCnvl(AssetHandle &quadMesh, const RenderState &state, Ref<FrameBuffer> &fbo)
	{
		Ref<QuadMesh> mesh = AssetsManager::GetRuntimeAsset<QuadMesh>(quadMesh);
		PIO_ASSERT_RETURN(mesh.use_count() != 0, "renderBrdfCnvl: Quad Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderProgram::IBL_BrdfConvl);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "renderBrdfCnvl: IBL_BrdfConvl shader is invalid");

		PIO_ASSERT_RETURN(fbo.use_count() != 0, "renderBrdfCnvl: fbo is invalid");

		shader->bind(true);

		mesh->VertexArray->bind();
		mesh->IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, mesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		if (GLHelper::CheckError("renderBrdfCnvl fail!!"))
			LOGD("succeed to create brdf lut texture");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		mesh->IndexBuffer->unbind();
		mesh->VertexArray->unbind();

		shader->bind(false);
	}

	void GLRenderAPI::beginRenderPass(Ref<RenderPass> &pass)
	{
		if (!pass)
			return;

		Ref<FrameBuffer> frameBuffer = pass->getSpecification().FrameBuffer;
		const RenderState &state = pass->getState();

		frameBuffer->bind();

		saveViewport(m_viewport);
		commitViewport(0, 0, frameBuffer->getWidth(), frameBuffer->getHeight());

		GLState::SetClear(state.Clear);
		submitRenderState(state);
	}

	void GLRenderAPI::endRenderPass(Ref<RenderPass> &pass)
	{
		if (!pass)
			return;

		pass->getSpecification().FrameBuffer->unbind();

		Viewport vp = restoreViewport();
		commitViewport(vp.X, vp.Y, vp.Width, vp.Height);
	}

	void GLRenderAPI::beginScreenPass(Ref<RenderPass> &pass, const Viewport &vp)
	{
		if (!pass)
			return;

		const RenderState &state = pass->getState();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		saveViewport(m_viewport);
		commitViewport(vp.X, vp.Y, vp.Width, vp.Height);

		GLState::SetClear(state.Clear);
		submitRenderState(state);
	}

	void GLRenderAPI::endScreenPass(Ref<RenderPass> &pass)
	{
		Viewport vp = restoreViewport();
		commitViewport(vp.X, vp.Y, vp.Width, vp.Height);
	}

	bool GLRenderAPI::UI_hasFocus(UiFocus focus)
	{
		auto &io = ImGui::GetIO();
		switch (focus)
		{
			case UiFocus::MouseCapture:
				return io.WantCaptureMouse;
			case UiFocus::KeyboardCapture:
				return io.WantCaptureKeyboard;
			case UiFocus::TextInputCapture:
				return io.WantTextInput;
			default:
				return false;
		}
	}

	void GLRenderAPI::initOpenGL()
	{
		PIO_ASSERT_RETURN(glewInit() == GLEW_OK, "glew fail to init, return");

		GLint major{ 0 }, minor{ 0 };
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		LOGD("glew succeed to init, version[%d.%d]", major, minor);
	}

	void GLRenderAPI::initImGui()
	{
		const char *glslVersion = "#version 430";

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		if (!ImGui::CreateContext())
		{
			LOGE("fail to create ImGui Context");
			return;
		}
		ImGuiIO &io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)Application::MainWindow()->getNativeWindow(), true);
		ImGui_ImplOpenGL3_Init(glslVersion);
		LOGD("init ImGui");
	}

	void GLRenderAPI::destroyImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		LOGD("destroy ImGui");
	}

	void GLRenderAPI::saveViewport(const Viewport &viewport)
	{
		m_viewportStack.push_back(viewport);
	}

	Viewport GLRenderAPI::restoreViewport()
	{
		Viewport ret;
		if (!m_viewportStack.empty())
		{
			ret = m_viewportStack.front();
			m_viewportStack.pop_front();
		}
		return ret;
	}

	void GLRenderAPI::drawMatPreview(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state, DrawMode drawMode)
	{
		Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(mesh.use_count() != 0, "drawMatPreview: fail to find mesh base from asset handle[%u]", (uint32_t)meshHandle);

		auto shader = ShaderLibrary::Get()->find(ShaderProgram::MaterialPreview);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "MaterialPreview shader is invalid");

		auto &meshSource = mesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));
		auto boneTransformUB = meshSource->getBoneTransformUB();

		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

		compareAndUpdateRenderState(m_globalState, state);
		if (state.Selected) { onPreOutlining(); }

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		cameraUB->bind();

		bool useNormalMap{ false };
		Ref<Material> material;
		if (submesh.MaterialIndex != NullIndex)
		{
			Ref<MaterialAsset> mtAsset = AssetsManager::GetRuntimeAsset<MaterialAsset>((*materialTable)[submesh.MaterialIndex]);
			if (mtAsset && (material = mtAsset->getMaterial()))
			{
				material->bind(shader);
				useNormalMap = material->getBool(MaterialAttrs::MU_UseNormalMap);
			}
		}

		Ref<BufferTexture> gpuAnimBuffer;
		if (boneTransformUB)
		{
			const BoneTransformUD &boneUD = meshSource->getBoneTransformUD();
			shader->setBool("u_bGpuAnimated", boneUD.GPUAnimated);
			if (boneUD.GPUAnimated)
			{
				shader->setInt("u_boneNum", boneUD.GPUTransform.BoneNum);
				shader->setInt("u_frameIndex", boneUD.GPUTransform.CurrentFrame);
				TextureSampler sampler{ TextureSampler::Slot0 };
				shader->getAvailableSampler(sampler);
				gpuAnimBuffer = boneUD.GPUTransform.BufferTexture;
				gpuAnimBuffer->active(PIO_UINT(sampler));
				gpuAnimBuffer->bind();
				shader->setInt("u_gpuAnimBuffer", PIO_INT(sampler));
			}
			else
			{
				UniformBuffer::Binding(shader, "BoneMatrices", boneTransformUB->getBinding());
				boneTransformUB->setData(boneUD.Block.getBuffer()->as<void *>(), boneUD.Block.getByteUsed());
				boneTransformUB->bind();
			}
		}

		shader->setMat4("u_modelMat", modelMat);
		shader->setBool("u_playAnimation", boneTransformUB.get());
		shader->setBool("u_useNormalMap", useNormalMap);

		shader->setBool("u_bOutline", false);
		shader->setBool("u_bStatic", mesh->is<StaticMesh>());
		shader->setFloat("u_outlineScale", Renderer::GetConfig().Debugger.OutlineScale);
		shader->setVec4("u_outlineColor", ColorSpace::sRGBToLinearRGB(Renderer::GetConfig().Debugger.OutlineColor));

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GetDrawMode(drawMode), submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("drawMatPreview fail!!");

#ifdef PIO_PROFILER_ON
		glFinish();
#endif // PIO_PROFILER_ON

		if(state.Selected)
		{
			StencilTest stencil;
			stencil.Enable = true;
			stencil.setMask(0x00);
			stencil.setFunc(StencilFunc{ FuncAttr::Notequal, 1, 0xff });
			GLState::SetStencilTest(stencil);

			shader->setBool("u_bOutline", true);

			glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			GLHelper::CheckError("drawMatPreview outline fail!!");

		#ifdef PIO_PROFILER_ON
			glFinish();
		#endif // PIO_PROFILER_ON

			//restore stencil test
			GLState::SetStencilTest(m_globalState.Stencil);
		}

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();		
		cameraUB->unbind();
		if (material) material->unbind();
		if (boneTransformUB) boneTransformUB->unbind();
		if (gpuAnimBuffer) gpuAnimBuffer->unbind();
		shader->bind(false);		
	}

	void GLRenderAPI::drawSubmesh_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(mesh.use_count() != 0, "fail to find mesh base from asset handle[%u]", (uint32_t)meshHandle);
		
		auto shader = ShaderLibrary::Get()->find(ShaderProgram::GeometryPass);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "GeometryPass shader is invalid");

		auto &meshSource = mesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));		
		auto boneTransformUB = meshSource->getBoneTransformUB();			

		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

		compareAndUpdateRenderState(m_globalState, state);
		if (state.Selected) { onPreOutlining(); }

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		cameraUB->bind();		

		bool useNormalMap{ false };
		Ref<Material> material;
		if (submesh.MaterialIndex != NullIndex)
		{
			Ref<MaterialAsset> mtAsset = AssetsManager::GetRuntimeAsset<MaterialAsset>((*materialTable)[submesh.MaterialIndex]);
			if (mtAsset && (material = mtAsset->getMaterial()))
			{
				material->bind(shader);
				useNormalMap = material->getBool(MaterialAttrs::MU_UseNormalMap);
			}
		}

		Ref<BufferTexture> gpuAnimBuffer;
		if (boneTransformUB)
		{
			const BoneTransformUD &boneUD = meshSource->getBoneTransformUD();
			shader->setBool("u_bGpuAnimated", boneUD.GPUAnimated);
			if (boneUD.GPUAnimated)
			{
				shader->setInt("u_boneNum", boneUD.GPUTransform.BoneNum);
				shader->setInt("u_frameIndex", boneUD.GPUTransform.CurrentFrame);
				TextureSampler sampler{ TextureSampler::Slot0 };
				shader->getAvailableSampler(sampler);
				gpuAnimBuffer = boneUD.GPUTransform.BufferTexture;
				gpuAnimBuffer->active(PIO_UINT(sampler));
				gpuAnimBuffer->bind();
				shader->setInt("u_gpuAnimBuffer", PIO_INT(sampler));
			}
			else
			{
				UniformBuffer::Binding(shader, "BoneMatrices", boneTransformUB->getBinding());
				boneTransformUB->setData(boneUD.Block.getBuffer()->as<void *>(), boneUD.Block.getByteUsed());
				boneTransformUB->bind();
			}
		}

		shader->setMat4("u_modelMat", modelMat);
		shader->setBool("u_playAnimation", boneTransformUB.get());
		shader->setBool("u_useNormalMap", useNormalMap);

		shader->setBool("u_bOutline", false);
		shader->setBool("u_bStatic", mesh->is<StaticMesh>());
		shader->setFloat("u_outlineScale", Renderer::GetConfig().Debugger.OutlineScale);
		shader->setVec4("u_outlineColor", ColorSpace::sRGBToLinearRGB(Renderer::GetConfig().Debugger.OutlineColor));

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("drawSubmesh_deferred fail!!");

	#ifdef PIO_PROFILER_ON
		glFinish();
	#endif // PIO_PROFILER_ON

		if(state.Selected)
		{
			StencilTest stencil;
			stencil.Enable = true;
			stencil.setMask(0x00);
			stencil.setFunc(StencilFunc{ FuncAttr::Notequal, 1, 0xff });
			GLState::SetStencilTest(stencil);

			shader->setBool("u_bOutline", true);
			
			glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			GLHelper::CheckError("drawSubmesh_deferred outline fail!!");

		#ifdef PIO_PROFILER_ON
			glFinish();
		#endif // PIO_PROFILER_ON

			//restore stencil test
			GLState::SetStencilTest(m_globalState.Stencil);
		}

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		if (material) material->unbind();

		cameraUB->unbind();
		if (boneTransformUB) boneTransformUB->unbind();
		if (gpuAnimBuffer) gpuAnimBuffer->unbind();
		shader->bind(false);		
	}

	void GLRenderAPI::submitRenderState(const RenderState &state)
	{
		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);
	}

	void GLRenderAPI::compareAndUpdateRenderState(RenderState &old, const RenderState &input)
	{
		if (old.Blend != input.Blend)
		{
			GLState::SetBlendMode(input.Blend);
			old.Blend = input.Blend;
		}

		if (old.DepthTest != input.DepthTest)
		{
			GLState::SetDepthTest(input.DepthTest);
			old.DepthTest = input.DepthTest;
		}

		if (old.Cull != input.Cull)
		{
			GLState::SetCullFace(input.Cull);
			old.Cull = input.Cull;
		}

		if (old.Stencil != input.Stencil)
		{
			GLState::SetStencilTest(input.Stencil);
			old.Stencil = input.Stencil;
		}
	}

	void GLRenderAPI::onPreOutlining()
	{
		// 1st. render pass for outline, 
		// update stencil buffer to 1s for each fragment drawn
		StencilTest stencil;
		stencil.Enable = true;
		stencil.setMask(0xff);
		stencil.setFunc(StencilFunc{ FuncAttr::Always, 1, 0xff });
		stencil.setOp(StencilOp{ FuncAttr::Keep, FuncAttr::Keep, FuncAttr::Replace });
		GLState::SetStencilTest(stencil);
	}

	uint32_t GLRenderAPI::GetDrawMode(DrawMode mode)
	{
		switch(mode)
		{
			case DrawMode::Lines:
				return GL_LINES;
			case DrawMode::Line_Strip:
				return GL_LINE_STRIP;
			case DrawMode::Line_Loop:
				return GL_LINE_LOOP;
			case DrawMode::Triangles:
				return GL_TRIANGLES;
			case DrawMode::Triangle_Strip:
				return GL_TRIANGLE_STRIP;
			case DrawMode::Triangle_Fan:
				return GL_TRIANGLE_FAN;
			default:
				LOGE("Err! DrawMode[%u] has not been implemented yet", mode);
				std::abort();
		}
	}
}