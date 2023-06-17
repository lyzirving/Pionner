#ifndef __PIONNER_GFX_FRAME_BUFFER_H__
#define __PIONNER_GFX_FRAME_BUFFER_H__

#include "GfxBuffer.h"

namespace Pionner
{
	class GfxFrameBuffer : public GfxBuffer
	{
	public:
		GfxFrameBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		GfxFrameBuffer(uint32_t width, uint32_t height, const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GfxFrameBuffer();

		uint32_t getAttachment(ATTACHMENT id) { return m_attachIds[id]; }

	protected:
		uint32_t m_width, m_height;
		uint32_t m_attachIds[ATTACHMENT_COUNT];
	};
}

#endif