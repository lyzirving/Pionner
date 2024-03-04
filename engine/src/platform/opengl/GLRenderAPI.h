#ifndef __PIONNER_PLATFORM_OPENGL_GL_RENDER_API_H__
#define __PIONNER_PLATFORM_OPENGL_GL_RENDER_API_H__

#include "gfx/rhi/RenderAPI.h"
#include "gfx/renderer/RenderState.h"

namespace pio
{
	class GLRenderAPI : public RenderAPI
	{
	public:
		GLRenderAPI();
		virtual ~GLRenderAPI();

		virtual void init() override;
		virtual void shutdown() override;

		virtual void beginFrame() override;
		virtual void endFrame() override;

		virtual void commitViewport(const Viewport &viewport) override;
		virtual void commitViewport(int32_t x, int32_t y, int32_t width, int32_t height) override;

		virtual void framebufferBlockTransfer(uint32_t readBuffer, uint32_t dstBuffer,
											  glm::ivec2 srcPos, glm::ivec2 srcSize,
											  glm::ivec2 dstPos, glm::ivec2 dstSize,
											  FrameBufferBlockBit flags, TextureFilterMag filter) override;

		virtual void renderSubmesh(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
								   Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) override;
		virtual void renderDistantLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
											  Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) override;
		virtual void renderPointLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
											Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) override;
		virtual void renderLightVolume(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &modelMat, Ref<UniformBufferSet> &uniformBufferSet, const RenderState &state) override;

		// ---------------------------- Deferred shading -------------------------------
		virtual void renderSubmesh_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable,
											Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) override;
		virtual void renderPointLightQuad_Deferred(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const Ref<RenderPass> &GBufferPass, const Ref<RenderPass> &shadowPass, const RenderState &state) override;
		virtual void renderDistantLightingQuad_Deferred(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const Ref<RenderPass> &GBufferPass, const Ref<RenderPass> &shadowPass, const RenderState &state) override;
		// -----------------------------------------------------------------------------

		// ------------------------------ 2D Rendering ---------------------------------
		virtual void renderLine(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &trans, const RenderState &state) override;
		virtual void renderTextureQuad2D(AssetHandle &meshHandle, Ref<Texture2D> &texture, const RenderState &state) override;
		virtual void renderSprite(const AssetHandle &quadMesh, const AssetHandle &texture, const RenderState &state) override;
		// -----------------------------------------------------------------------------

		// ------------------------------- Tools -------------------------------------
		virtual void renderSkybox(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, Ref<CubeTexture> &cubeTexture, const RenderState &state) override;
		virtual void renderHDRToCube(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment cubeAttach, Ref<Texture2D> &HDRTexture, Ref<FrameBuffer> &cubeFbo) override;
		virtual void postprocessing(const AssetHandle &meshHandle, Ref<Texture2D> &composite, const RenderState &state) override;
		// ---------------------------------------------------------------------------

		virtual void beginRenderPass(Ref<RenderPass> &pass) override;
		virtual void endRenderPass(Ref<RenderPass> &pass) override;

		virtual void beginScreenPass(Ref<RenderPass> &pass, const Viewport &vp) override;
		virtual void endScreenPass(Ref<RenderPass> &pass) override;

		virtual bool UI_hasFocus(UiFocus focus) override;
		
	private:
		void initOpenGL();
		void initImGui();

		void destroyImGui();

		void saveViewport(const Viewport &viewport);
		Viewport restoreViewport();

		void drawPBRSubMesh(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
						    Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);
		void drawMatPreview(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, 
							Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);
		void drawWireframe(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable,
						   Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);

		// ---------------------------- Deferred shading -------------------------------
		void drawSubmesh_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable,
								  Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);
		// -----------------------------------------------------------------------------

		void onPreOutlining(const RenderState &state);
		void onOutlining(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);
		void onOutlining_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);

	private:
		RenderState m_globalState{};
		Viewport m_viewport{};

		std::list<Viewport> m_viewportStack;
	};
}

#endif