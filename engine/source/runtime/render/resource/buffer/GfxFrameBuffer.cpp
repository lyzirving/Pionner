#include "GfxFrameBuffer.h"

namespace pio
{
	GfxFrameBuffer::GfxFrameBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr), m_width(0), m_height(0)
		, m_attachIds()
	{
		m_attachIds[ATTACHMENT_COUNT - 1] = { 0 };
	}

	GfxFrameBuffer::GfxFrameBuffer(uint32_t width, uint32_t height, const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr), m_width(width), m_height(height)
		, m_attachIds()
	{
		m_attachIds[ATTACHMENT_COUNT - 1] = { 0 };
	}

	GfxFrameBuffer::~GfxFrameBuffer()
	{
		notifyRelease();
	}

}