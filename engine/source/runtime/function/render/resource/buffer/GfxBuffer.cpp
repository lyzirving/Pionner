#include "function/render/resource/buffer/GfxBuffer.h"
#include "GfxBuffer.h"

namespace Pionner
{
	Pionner::GfxBuffer::GfxBuffer()
		: m_id(0), m_type(BUF_CNT), m_loaded(false)
	{
	}

	GfxBuffer::~GfxBuffer()
	{
	}
}