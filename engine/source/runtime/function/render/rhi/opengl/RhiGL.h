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

	protected:
		virtual void createInstance() override;
	};
}

#endif