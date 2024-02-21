#ifndef __PIONNER_GFX_RHI_RENDER_PASS_H__
#define __PIONNER_GFX_RHI_RENDER_PASS_H__

#include "gfx/rhi/RhiDef.h"
#include "gfx/renderer/RenderState.h"

namespace pio
{
	class FrameBuffer;
	class UniformBuffer;
	class UniformBufferSet;
	class Camera;
	class Texture2D;
	struct LightEnvironment;
	struct SpriteCommand;
	struct MeshKey;

	struct RenderPassSpecification
	{
		std::string Name;
		Ref<FrameBuffer> FrameBuffer;
		Ref<UniformBuffer> DataUB;
	};

	class RenderPass
	{
		PIO_UNIFORM_WRAPPER_INTERFACE_DECLARE(RenderPass)
	public:
		virtual ~RenderPass() = default;

		virtual void setState(const RenderState &state) = 0;

		virtual const RenderPassSpecification &getSpecification() const = 0;
		virtual const RenderState &getState() const = 0;
		virtual Ref<FrameBuffer> getFramebuffer() = 0;
		virtual Ref<UniformBuffer> getUniformBuffer() = 0;
		virtual Camera &getCamera() = 0;

	public:
		static Ref<RenderPass> Create(const RenderPassSpecification &spec);

		static void RenderLightingEffect_Deferred(const LightEnvironment &lightEnv, Ref<RenderPass> &GBufferPass, Ref<RenderPass> &distantLightShadowPass, Ref<RenderPass> &pointLightShadowPass, Ref<UniformBufferSet> &uniformBufferSet);
		static void RenderSprites(std::map<MeshKey, SpriteCommand> &cmd);
		static void Postprocessing(const AssetHandle &quadMesh, Ref<Texture2D> &composite);
	};
}

#endif