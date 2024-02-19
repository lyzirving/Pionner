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

#include "gfx/struct/MaterialAsset.h"
#include "gfx/struct/ShaderLibrary.h"
#include "gfx/struct/Geometry2D.h"
#include "gfx/struct/MeshUtil.h"
#include "gfx/struct/Camera.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"
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
		commitViewport(0, 0, Application::MainWindow()->getWidth(), Application::MainWindow()->getHeight());
		GLState::SetClear(m_globalState.Clear);
		GLState::SetDepthTest(m_globalState.DepthTest);

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

	void GLRenderAPI::endFrame()
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
			case RenderMode::PBR:
			{
				drawPBRSubMesh(meshHandle, submeshIndex, materialTable, shadowPass, uniformBufferSet, modelMat, state);
				break;
			}
			case RenderMode::MaterialPreview:
			{
				drawMatPreview(meshHandle, submeshIndex, materialTable, uniformBufferSet, modelMat, state);
				break;
			}
			case RenderMode::Wireframe:
			{
				drawWireframe(meshHandle, submeshIndex, materialTable, uniformBufferSet, modelMat, state);
				break;
			}
			case RenderMode::Blinn_Phong:
			default:
				LOGE("render mode[%u] has not been implemented", state.Mode);
				break;
		}
	}

	void GLRenderAPI::renderDistantLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		PIO_ASSERT_RETURN(shadowPass.use_count() != 0, "renderDistantLightShadow: invalid shadow pass");

		Ref<MeshBase> meshBase = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(meshBase.use_count() != 0, "renderDistantLightShadow: fail to get mesh from[%u]", (uint32_t)meshHandle);

		auto shader = ShaderLibrary::Get()->find(ShaderType::DistantLight_ShadowData);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "DistantLight_ShadowData shader is invalid");

		Ref<MeshSource> meshSource = meshBase->getMeshSource();

		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];
		Ref<UniformBuffer> boneTransformUB = meshSource->getBoneTransformUB();
		Ref<UniformBuffer> sdUB = uniformBufferSet->get(PIO_UINT(UBBindings::DistantLightShadowData));

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);

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

		auto shader = ShaderLibrary::Get()->find(ShaderType::PointLight_ShadowData);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "PointLight_ShadowData shader is invalid");

		Ref<MeshSource> meshSource = meshBase->getMeshSource();

		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];
		Ref<UniformBuffer> boneTransformUB = meshSource->getBoneTransformUB();

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);

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

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		ptShadowDataUB->unbind();
		if (boneTransformUB) boneTransformUB->unbind();
		if (gpuAnimBuffer) gpuAnimBuffer->unbind();
		shader->bind(false);
	}

	void GLRenderAPI::renderLightVolume(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &modelMat, Ref<UniformBufferSet> &uniformBufferSet, const RenderState &state)
	{
		Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(mesh.use_count() != 0, "renderLightVolume: fail to find mesh base from asset handle[%u]", (uint32_t)meshHandle);

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::LightVolume);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "renderLightVolume: fail to shader [LightVolume]");

		Ref<MeshSource> meshSource = mesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];
		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

		shader->bind(true);

		Ref<UniformBuffer> cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));
		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		cameraUB->bind();

		shader->setMat4("u_modelMat", modelMat);

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderLightVolume fail!!");

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		cameraUB->unbind();
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
			case RenderMode::Blinn_Phong:
			default:
				LOGE("render mode[%u] has not been implemented", state.Mode);
				break;
		}
	}

	void GLRenderAPI::renderPointLightQuad_Deferred(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const Ref<RenderPass> &GBufferPass, const Ref<RenderPass> &shadowPass, const RenderState &state)
	{
		Ref<QuadMesh> quadMesh = AssetsManager::GetRuntimeAsset<QuadMesh>(meshHandle);
		PIO_ASSERT_RETURN(quadMesh.use_count() != 0, "renderPointLightQuad_Deferred: Quad Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::LightingEffect_Deferred);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "LightingEffect_Deferred shader is invalid");

		PIO_ASSERT_RETURN(GBufferPass.use_count() != 0, "GBuffer pass is invalid");
		Ref<FrameBuffer> gBuffer = GBufferPass->getSpecification().FrameBuffer;

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));
		auto pointLightDataUB = uniformBufferSet->get(PIO_UINT(UBBindings::PointLightData));

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		UniformBuffer::Binding(shader, "PointLightData", pointLightDataUB->getBinding());

		cameraUB->bind();
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

		TextureSampler depthSlot;
		Ref<Texture> depthBuffer;
		if (shadowPass)
		{
			depthBuffer = shadowPass->getSpecification().FrameBuffer->getDepthBuffer();
			if (depthBuffer && shader->getAvailableSampler(depthSlot))
			{
				shader->setTextureSampler("u_pointLightDepthBuffer", depthSlot);
				depthBuffer->active(PIO_UINT(depthSlot));
				depthBuffer->bind();
			}
		}

		shader->setVec4("u_bgColor", Renderer::GetConfig().ClearColor);

		quadMesh->VertexArray->bind();
		quadMesh->IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, quadMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderPointLightQuad_Deferred fail!!");

		quadMesh->IndexBuffer->unbind();
		quadMesh->VertexArray->unbind();

		cameraUB->unbind();
		pointLightDataUB->unbind();
		positionBuf->unbind();
		normalBuf->unbind();
		albedoBuf->unbind();
		matBuf->unbind();
		emissionBuf->unbind();
		if (depthBuffer) { depthBuffer->unbind(); }

		shader->bind(false);
	}

	void GLRenderAPI::renderDistantLightingQuad_Deferred(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const Ref<RenderPass> &GBufferPass, const Ref<RenderPass> &shadowPass, const RenderState &state)
	{
		Ref<QuadMesh> quadMesh = AssetsManager::GetRuntimeAsset<QuadMesh>(meshHandle);
		PIO_ASSERT_RETURN(quadMesh.use_count() != 0, "renderDistantLightingQuad_Deferred: Quad Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::DistantLighting_Deferred);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "DistantLighting_Deferred shader is invalid");

		PIO_ASSERT_RETURN(GBufferPass.use_count() != 0, "GBuffer pass is invalid");
		Ref<FrameBuffer> gBuffer = GBufferPass->getFramebuffer();

		PIO_ASSERT_RETURN(shadowPass.use_count() != 0, "shadow pass is invalid");
		Ref<Texture> shadowBuffer = shadowPass->getFramebuffer()->getDepthBuffer();

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));
		auto dirLightUB = uniformBufferSet->get(PIO_UINT(UBBindings::DistantLight));
		auto dirLightSdUB = uniformBufferSet->get(PIO_UINT(UBBindings::DistantLightShadowData));

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		UniformBuffer::Binding(shader, "DirectionalLight", dirLightUB->getBinding());
		UniformBuffer::Binding(shader, "SdMatrices", dirLightSdUB->getBinding());

		cameraUB->bind();
		dirLightUB->bind();
		dirLightSdUB->bind();

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

		shader->setVec4("u_bgColor", Renderer::GetConfig().ClearColor);

		quadMesh->VertexArray->bind();
		quadMesh->IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, quadMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderDistantLightingQuad_Deferred fail!!");

		quadMesh->IndexBuffer->unbind();
		quadMesh->VertexArray->unbind();

		cameraUB->unbind();
		dirLightUB->unbind();
		dirLightSdUB->unbind();

		positionBuf->unbind();
		normalBuf->unbind();
		albedoBuf->unbind();
		matBuf->unbind();
		emissionBuf->unbind();

		shader->bind(false);
	}

	void GLRenderAPI::renderLine(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const RenderState &state)
	{
		Ref<LineMesh> lineMesh = AssetsManager::GetRuntimeAsset<LineMesh>(meshHandle);
		PIO_ASSERT_RETURN(lineMesh.use_count() != 0, "Line Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::Color_Line);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "ColorLine shader is invalid");

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		cameraUB->bind();

		lineMesh->VertexArray->bind();
		lineMesh->IndexBuffer->bind();

		glDrawElements(GL_LINES, lineMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderLine fail!!");

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

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

		shader->bind(true);

		texture->active(PIO_UINT(TextureSampler::Slot0));
		texture->bind();

		shader->setTextureSampler("u_quadTexture", TextureSampler::Slot0);

		quadMesh->VertexArray->bind();
		quadMesh->IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, quadMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("renderTextureQuad2D fail!!");

		quadMesh->IndexBuffer->unbind();
		quadMesh->VertexArray->unbind();

		texture->unbind();

		shader->bind(false);
	}

	void GLRenderAPI::renderSkybox(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, Ref<CubeTexture> &cubeTexture, const RenderState &state)
	{
		Ref<MeshBase> cubeMesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(cubeMesh.use_count() != 0, "renderSkybox: Cube Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::Skybox);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "Skybox shader is invalid");

		PIO_ASSERT_RETURN(cubeTexture.use_count() != 0, "renderSkybox: cubeTexture is invalid");

		Ref<MeshSource> meshSource = cubeMesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];
		Ref<UniformBuffer> cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

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

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		cameraUB->unbind();
		cubeTexture->unbind();
		shader->bind(false);
	}

	void GLRenderAPI::renderHDRToCube(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment cubeAttach, Ref<Texture2D> &HDRTexture, Ref<FrameBuffer> &cubeFbo)
	{
		Ref<MeshBase> cubeMesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(cubeMesh.use_count() != 0, "renderHDRToCube: Cube Mesh is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::Equirectangular_To_Cube);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "Equirectangular_To_Cube shader is invalid");

		PIO_ASSERT_RETURN(HDRTexture.use_count() != 0, "renderHDRToCube: HDRTexture is invalid");
		PIO_ASSERT_RETURN(cubeFbo.use_count() != 0, "renderHDRToCube: cubeFbo is invalid");

		Ref<MeshSource> meshSource = cubeMesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		shader->bind(true);

		HDRTexture->active(PIO_UINT(TextureSampler::Slot0));
		HDRTexture->bind();

		shader->setTextureSampler("u_equirectangularMap", TextureSampler::Slot0);
		shader->setMat4("u_prjMat", prjMat);

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		for (uint8_t i = 0; i < LightDir_Num; i++)
		{			
			cubeFbo->bindTarget(cubeAttach, LightDir(i));
			GLState::SetClear(Clear::Common());
			shader->setMat4("u_viewMat", viewMat[i]);			
			glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			if (GLHelper::CheckError("renderHDRToCube fail at light dir[%u]!!", i))
				LOGD("succeed to render HDR to cube face[%s]", LightDirStr(LightDir(i)));
		}		

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		HDRTexture->unbind();
		shader->bind(false);
	}

	void GLRenderAPI::postprocessing(AssetHandle &meshHandle, Ref<Texture2D> &composite, const RenderState &state)
	{
		Ref<QuadMesh> quadMesh = AssetsManager::GetRuntimeAsset<QuadMesh>(meshHandle);
		PIO_ASSERT_RETURN(quadMesh.use_count() != 0, "postprocessing: Quad Mesh is invalid");

		PIO_ASSERT_RETURN(composite.use_count() != 0, "postprocessing: composite texture is invalid");

		Ref<Shader> shader = ShaderLibrary::Get()->find(ShaderType::Postprocessing);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "postprocessing: shader is invalid");

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

		shader->bind(true);

		composite->active(PIO_UINT(TextureSampler::Slot0));
		composite->bind();
		shader->setTextureSampler("u_composite", TextureSampler::Slot0);

		quadMesh->VertexArray->bind();
		quadMesh->IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, quadMesh->IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("postprocessing fail!!");

		quadMesh->IndexBuffer->unbind();
		quadMesh->VertexArray->unbind();

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
		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);
	}

	void GLRenderAPI::endRenderPass(Ref<RenderPass> &pass)
	{
		if (!pass)
			return;

		pass->getSpecification().FrameBuffer->unbind();

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

		m_globalState.Blend = Blend::Common();
		m_globalState.Cull = CullFace::Common();
		m_globalState.DepthTest = DepthTest::Common();
		// Global Background Color
		m_globalState.Clear = Clear::Common(glm::vec4(0.f, 0.f, 0.f, 1.f));
		m_globalState.Stencil.Enable = true;
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

	void GLRenderAPI::drawPBRSubMesh(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(mesh.use_count() != 0, "drawPBRSubMesh: fail to find mesh base from asset handle[%u]", (uint32_t)meshHandle);

		auto shader = ShaderLibrary::Get()->find(ShaderType::PBR_Mesh);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "PBR_Mesh shader is invalid");

		auto &meshSource = mesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		auto boneTransformUB = meshSource->getBoneTransformUB();
		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));
		auto pointLightDataUB = uniformBufferSet->get(PIO_UINT(UBBindings::PointLightData));

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

		onPreOutlining(state);

		shader->bind(true);

		UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
		UniformBuffer::Binding(shader, "PointLightData", pointLightDataUB->getBinding());

		cameraUB->bind();
		pointLightDataUB->bind();

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

		TextureSampler depthSlot;
		Ref<Texture> depthBuffer;
		if (shadowPass)
		{
			depthBuffer = shadowPass->getSpecification().FrameBuffer->getDepthBuffer();
			if (depthBuffer && shader->getAvailableSampler(depthSlot))
			{
				shader->setTextureSampler("u_pointLightDepthBuffer", depthSlot);
				depthBuffer->active(PIO_UINT(depthSlot));
				depthBuffer->bind();
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

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("drawPBRSubMesh fail!!");

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		cameraUB->unbind();
		if (material) material->unbind();
		if (depthBuffer) depthBuffer->unbind();
		if (boneTransformUB) { boneTransformUB->unbind(); }
		if (gpuAnimBuffer) gpuAnimBuffer->unbind();

		shader->bind(false);

		onOutlining(meshHandle, submeshIndex, uniformBufferSet, modelMat, state);
	}

	void GLRenderAPI::drawMatPreview(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(mesh.use_count() != 0, "drawMatPreview: fail to find mesh base from asset handle[%u]", (uint32_t)meshHandle);

		auto shader = ShaderLibrary::Get()->find(ShaderType::MaterialPreview);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "MaterialPreview shader is invalid");

		auto &meshSource = mesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));
		auto boneTransformUB = meshSource->getBoneTransformUB();

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

		onPreOutlining(state);

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

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("drawMatPreview fail!!");

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();		
		cameraUB->unbind();
		if (material) material->unbind();
		if (boneTransformUB) boneTransformUB->unbind();
		if (gpuAnimBuffer) gpuAnimBuffer->unbind();
		shader->bind(false);

		onOutlining(meshHandle, submeshIndex, uniformBufferSet, modelMat, state);
	}

	void GLRenderAPI::drawWireframe(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
		PIO_ASSERT_RETURN(mesh.use_count() != 0, "drawWireframe: fail to find mesh base from asset handle[%u]", (uint32_t)meshHandle);

		auto shader = ShaderLibrary::Get()->find(ShaderType::Wireframe);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "Wireframe shader is invalid");

		auto &meshSource = mesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));
		auto boneTransformUB = meshSource->getBoneTransformUB();

		GLState::SetBlendMode(state.Blend);
		GLState::SetDepthTest(state.DepthTest);
		GLState::SetCullFace(state.Cull);
		GLState::SetStencilTest(state.Stencil);

		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

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

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_LINE_STRIP, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("drawWireframe fail!!");

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

		auto shader = ShaderLibrary::Get()->find(ShaderType::Mesh_Deferred);
		PIO_ASSERT_RETURN(shader.use_count() != 0, "Mesh_Deferred shader is invalid");

		auto &meshSource = mesh->getMeshSource();
		const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

		auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));		
		auto boneTransformUB = meshSource->getBoneTransformUB();			

		if (meshSource->is<Geometry>()) { meshSource->as<Geometry>()->flush(); }

		onPreOutlining(state);

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

		submesh.VertexArray->bind();
		submesh.IndexBuffer->bind();

		glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
		GLHelper::CheckError("drawSubmesh_deferred fail!!");

		submesh.IndexBuffer->unbind();
		submesh.VertexArray->unbind();

		if (material) material->unbind();

		cameraUB->unbind();
		if (boneTransformUB) boneTransformUB->unbind();
		if (gpuAnimBuffer) gpuAnimBuffer->unbind();
		shader->bind(false);

		onOutlining_deferred(meshHandle, submeshIndex, uniformBufferSet, modelMat, state);
	}

	void GLRenderAPI::onPreOutlining(const RenderState &state)
	{
		if (state.Selected)
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
	}

	void GLRenderAPI::onOutlining(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		if (state.Selected)
		{
			// 2nd. render pass for outline, draw slightly scaled versions of the objects
			// Now stencil buffer is filled with 1s. 
			// The parts of the buffer that are 1 are not drawn, thus only drawing 
			// the objects' size differences, making it look like borders.
			Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
			auto shader = ShaderLibrary::Get()->find(ShaderType::Outline);

			auto &meshSource = mesh->getMeshSource();
			const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

			auto boneTransformUB = meshSource->getBoneTransformUB();
			auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));

			StencilTest stencil;
			stencil.Enable = true;
			stencil.setMask(0x00);
			stencil.setFunc(StencilFunc{ FuncAttr::Notequal, 1, 0xff });
			GLState::SetStencilTest(stencil);

			shader->bind(true);

			UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
			cameraUB->bind();

			if (boneTransformUB)
			{
				UniformBuffer::Binding(shader, "BoneMatrices", boneTransformUB->getBinding());
				const BoneTransformUD &boneUD = meshSource->getBoneTransformUD();
				boneTransformUB->setData(boneUD.Block.getBuffer()->as<void *>(), boneUD.Block.getByteUsed());
				boneTransformUB->bind();
			}

			shader->setMat4("u_modelMat", modelMat);
			shader->setBool("u_playAnimation", boneTransformUB.get());
			shader->setBool("u_bStatic", mesh->is<StaticMesh>());
			shader->setFloat("u_outlineScale", Renderer::GetConfig().Debugger.OutlineScale);
			shader->setVec4("u_outlineColor", ColorSpace::sRGBToLinearRGB(Renderer::GetConfig().Debugger.OutlineColor));

			submesh.VertexArray->bind();
			submesh.IndexBuffer->bind();

			glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			GLHelper::CheckError("onOutlining fail!!");

			submesh.IndexBuffer->unbind();
			submesh.VertexArray->unbind();

			cameraUB->unbind();
			if (boneTransformUB) boneTransformUB->unbind();

			shader->bind(false);

			//restore stencil test
			GLState::SetStencilTest(m_globalState.Stencil);
		}
	}

	void GLRenderAPI::onOutlining_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		if (state.Selected)
		{
			Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(meshHandle);
			auto shader = ShaderLibrary::Get()->find(ShaderType::Outline_Deferred);

			auto &meshSource = mesh->getMeshSource();
			const Submesh &submesh = meshSource->getSubmeshes()[submeshIndex];

			auto boneTransformUB = meshSource->getBoneTransformUB();
			auto cameraUB = uniformBufferSet->get(PIO_UINT(UBBindings::Camera));

			StencilTest stencil;
			stencil.Enable = true;
			stencil.setMask(0x00);
			stencil.setFunc(StencilFunc{ FuncAttr::Notequal, 1, 0xff });
			GLState::SetStencilTest(stencil);

			shader->bind(true);

			UniformBuffer::Binding(shader, "Matrices", cameraUB->getBinding());
			cameraUB->bind();

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
			shader->setBool("u_bStatic", mesh->is<StaticMesh>());
			shader->setFloat("u_outlineScale", Renderer::GetConfig().Debugger.OutlineScale);
			shader->setVec4("u_outlineColor", ColorSpace::sRGBToLinearRGB(Renderer::GetConfig().Debugger.OutlineColor));

			submesh.VertexArray->bind();
			submesh.IndexBuffer->bind();

			glDrawElements(GL_TRIANGLES, submesh.IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			GLHelper::CheckError("onOutlining_deferred fail!!");

			submesh.IndexBuffer->unbind();
			submesh.VertexArray->unbind();

			cameraUB->unbind();
			if (boneTransformUB) boneTransformUB->unbind();
			if (gpuAnimBuffer) gpuAnimBuffer->unbind();
			shader->bind(false);

			//restore stencil test
			GLState::SetStencilTest(m_globalState.Stencil);
		}
	}
}