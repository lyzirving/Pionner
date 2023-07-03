#include "RenderPassBase.h"
#include "render/rhi/Rhi.h"

#include "window/ui/WindowUI.h"

namespace pio
{
	RenderPassBase::RenderPassBase() : m_rhi(nullptr)
	{
	}

	RenderPassBase::~RenderPassBase()
	{
		m_rhi.reset();
	}

}