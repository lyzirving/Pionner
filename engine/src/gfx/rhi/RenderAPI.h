#ifndef __PIONNER_GFX_RHI_RENDER_API_H__
#define __PIONNER_GFX_RHI_RENDER_API_H__

#include "RhiDef.h"
#include "gfx/renderer/RenderData.h"
#include "scene/SceneDef.h"
#include "asset/Asset.h"

namespace pio
{
	class MaterialTable;
	class RenderPass;
	class UniformBufferSet;
	class Texture2D;
	class CubeTexture;
	class FrameBuffer;
	class Skybox;
	struct Viewport;

	class RenderAPI
	{
	public:
		enum class Type
		{
			None,
			OpenGL,
			Vulkan //TODO: support vulkan
		};

	public:
		RenderAPI() {}
		virtual ~RenderAPI() = default;

		virtual void init() = 0;
		virtual void shutdown() = 0;

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		virtual void beginUI() = 0;
		virtual void endUI() = 0;

		virtual void commitViewport(const Viewport &viewport) = 0;
		virtual void commitViewport(int32_t x, int32_t y, int32_t width, int32_t height) = 0;

		virtual void framebufferBlockTransfer(uint32_t readBuffer, uint32_t dstBuffer,
											  glm::ivec2 srcPos, glm::ivec2 srcSize,
											  glm::ivec2 dstPos, glm::ivec2 dstSize,
											  FrameBufferBlockBit flags, TextureFilterMag filter) = 0;

		virtual void renderSubmesh(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
								   Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) = 0;	

		virtual void renderDistantLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
											  Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) = 0;
		virtual void renderPointLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
									        Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) = 0;	

		// ---------------------------- Deferring shading ----------------------------
		virtual void renderSubmesh_deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable,
											Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state) = 0;
		virtual void renderLightEffect_Deffered(AssetHandle& meshHandle, Ref<Skybox>& skybox, Ref<UniformBufferSet>& ubs, const Ref<RenderPass>& GBufferPass,
			                                    const Ref<RenderPass>& shadowPass, const Ref<RenderPass>& ptShadowPass, const RenderState& state) = 0;
		// ---------------------------------------------------------------------------

		// ------------------------------ 2D Rendering -------------------------------		
		virtual void renderLineSegment(const AssetHandle &h, const glm::vec4 &color, const Ref<UniformBufferSet> &ubs, const glm::mat4 &trans, const RenderState &state) = 0;		
		virtual void renderSprites(const std::vector<SpriteCommand> &cmds) = 0;
		virtual void renderSprites(const std::vector<TextureCmd> &cmds) = 0;
		// ---------------------------------------------------------------------------

		// ------------------------------- Tools -------------------------------------
		virtual void renderSkybox(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, Ref<CubeTexture> &cubeTexture, const RenderState &state) = 0;
		virtual void renderHDRToEnvMap(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment envMapAttachment, Ref<Texture2D> &HDRTexture, Ref<FrameBuffer> &fbo) = 0;
		virtual void renderDiffuseCnvl(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment diffuseMapAttachment, Ref<CubeTexture> &envMap, Ref<FrameBuffer> &fbo) = 0;
		virtual void renderBrdfCnvl(AssetHandle &quadMesh, const RenderState &state, Ref<FrameBuffer> &fbo) = 0;		
		// ---------------------------------------------------------------------------

		virtual void beginRenderPass(Ref<RenderPass> &pass) = 0;
		virtual void endRenderPass(Ref<RenderPass> &pass) = 0;

		virtual void beginScreenPass(Ref<RenderPass> &pass, const Viewport& vp) = 0;
		virtual void endScreenPass(Ref<RenderPass> &pass) = 0;

		virtual bool UI_hasFocus(UiFocus focus) = 0;

	public:
		static bool IsOpenGL() { return CurrentType() == Type::OpenGL; }
		static Type CurrentType() { return s_type; }
		static void SetType(Type type);

	private:
		static Type s_type;
	};
}

#endif