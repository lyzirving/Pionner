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

		virtual void beginUI() override;
		virtual void endUI() override;

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

		// ---------------------------- Deferred shading -------------------------------
		virtual void renderSubmesh_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable,
											Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) override;
		virtual void renderLightEffect_Deffered(AssetHandle& meshHandle, Ref<Skybox>& skybox, Ref<UniformBufferSet>& ubs, const Ref<RenderPass>& GBufferPass,
			                                    const Ref<RenderPass>& shadowPass, const Ref<RenderPass>& ptShadowPass, const RenderState& state) override;
		// -----------------------------------------------------------------------------

		// ------------------------------ 2D Rendering ---------------------------------		
		virtual void renderLineSegment(const AssetHandle &h, const glm::vec4 &color, const Ref<UniformBufferSet> &ubs, const glm::mat4 &trans, const RenderState &state) override;		
		virtual void renderSprites(const std::vector<SpriteCommand> &cmds) override;
		virtual void renderSprites(const std::vector<TextureCmd> &cmds) override;
		// -----------------------------------------------------------------------------

		// ------------------------------- Tools -------------------------------------
		virtual void renderSkybox(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, Ref<CubeTexture> &cubeTexture, const RenderState &state) override;
		virtual void renderHDRToEnvMap(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment envMapAttachment, Ref<Texture2D> &HDRTexture, Ref<FrameBuffer> &fbo) override;
		virtual void renderDiffuseCnvl(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment diffuseMapAttachment, Ref<CubeTexture> &envMap, Ref<FrameBuffer> &fbo) override;		
		virtual void renderBrdfCnvl(AssetHandle &quadMesh, const RenderState &state, Ref<FrameBuffer> &fbo) override;		
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

		// ----------------------------- Forward shading -------------------------------
		void drawMatPreview(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, 
							Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state, 
							DrawMode drawMode = DrawMode::Triangles);	
		// -----------------------------------------------------------------------------

		// ---------------------------- Deferred shading -------------------------------
		void drawSubmesh_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable,
								  Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);
		// -----------------------------------------------------------------------------

		void submitRenderState(const RenderState &state);
		void compareAndUpdateRenderState(RenderState &old, const RenderState &input);

		void onPreOutlining();

	private:
		static uint32_t GetDrawMode(DrawMode mode);

	private:		
		const RenderState m_defaultState{ Clear::Common(glm::vec4(0.f, 0.f, 0.f, 1.f)),
										  Blend::Disable(), DepthTest::Common(),
										  CullFace::Common(), StencilTest::Common(),
										  RenderMode::PBR, false };
		RenderState m_globalState{};

		Viewport m_viewport{};
		std::list<Viewport> m_viewportStack;
	};
}

#endif