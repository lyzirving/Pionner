#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__

#include <memory>

#include "function/render/resource/ResourceDef.h"

namespace Pionner
{
	class Rhi;
	class RenderResourceMgr;

	class GfxBuffer
	{
	public:
		GfxBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GfxBuffer();

		virtual void upload() = 0;
		virtual void release() = 0;

		virtual void bindToTarget(uint32_t target);

	protected:
		friend class RenderResourceMgr;

		inline bool isCreated() { return m_id > 0; }

		std::weak_ptr<RenderResourceMgr> m_mgr;
		uint32_t                         m_id;
		BufferType                       m_type;
		bool                             m_loaded;
	};
}

#endif