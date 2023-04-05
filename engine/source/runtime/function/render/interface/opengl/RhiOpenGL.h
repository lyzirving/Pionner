#ifndef __RHI_OPENGL_H__
#define __RHI_OPENGL_H__

#include "function/render/interface/Rhi.h"

namespace Pionner
{
	class RhiOpenGL : public Rhi
	{
	public:
		RhiOpenGL();
		virtual ~RhiOpenGL();

		virtual void initialize(void* param) override;
		virtual void initUIRenderBackend() override;
		virtual void shutdownUIRenderBackend() override;
		virtual void shutdown() override;

	protected:
		virtual void createInstance() override;
	};
}

#endif