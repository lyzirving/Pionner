#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__

#include <memory>

#include "function/render/resource/ResourceDef.h"

namespace Pionner
{
	class Rhi;

	class GfxBuffer
	{
	public:
		GfxBuffer();
		virtual ~GfxBuffer();

		virtual void upload(const std::shared_ptr<Rhi> &rhi) = 0;
		virtual void release(const std::shared_ptr<Rhi> &rhi) = 0;

	protected:
		friend class RenderResourceMgr;

		uint32_t   m_id;
		BufferType m_type;
		bool       m_loaded;
	};
}

#endif