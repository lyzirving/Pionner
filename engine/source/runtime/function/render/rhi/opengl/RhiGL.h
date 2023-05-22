#ifndef __RHI_GL_H__
#define __RHI_GL_H__

#include "function/render/rhi/Rhi.h"

namespace Pionner
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
		virtual std::shared_ptr<ShaderRhi> getShaderRhi() override;
		virtual std::shared_ptr<DrawCmd> getDrawCmd() override;

		virtual void viewportFull() override;
		virtual void viewportSub(int x, int y, int width, int height) override;

		virtual void setClearMode(ClearMode &mode) override;
		virtual void setCullMode(CullFace &mode) override;
		virtual void setDepthMode(DepthTest &test) override;

	protected:
		virtual void createInstance() override;

	private:
		static uint32_t getGLFaceDir(FaceDirection dir);
		static uint32_t getGLCullMode(CullMode mode);
	};
}

#endif