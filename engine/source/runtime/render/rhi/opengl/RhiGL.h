#ifndef __RHI_GL_H__
#define __RHI_GL_H__

#include "render/rhi/Rhi.h"

namespace pio
{
	class RhiGL : public Rhi
	{
	public:
		RhiGL();
		virtual ~RhiGL();

		virtual void initialize(void *param) override;
		virtual void initUIRenderBackend() override;
		virtual void shutdownUIRenderBackend() override;
		virtual void shutdown() override;
		virtual std::shared_ptr<DrawCmd> getDrawCmd() override;

		virtual void clear(uint32_t flags) override;
		virtual void clearColor(const glm::vec4 &color) override;

		virtual void setViewport(int x, int y, int width, int height) override;
		virtual void setCullMode(gfx::CullFace &mode) override;
		virtual void setDepthMode(gfx::DepthTest &test) override;
		virtual void setBlendMode(gfx::Blend &blend) override;

		virtual void unbindBufSlot(BufferType type) override;

	protected:
		virtual void createInstance() override;

	private:
		static uint32_t getGLFaceDir(gfx::FaceDirection dir);
		static uint32_t getGLCullMode(gfx::CullMode mode);
		static uint32_t getGLBlendFactor(gfx::BlendFactor blendFactor);
	};
}

#endif