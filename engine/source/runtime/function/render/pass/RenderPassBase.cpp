#include "RenderPassBase.h"
#include "function/render/rhi/Rhi.h"

#include "function/ui/WindowUI.h"

namespace Pionner
{
	RenderPassBase::RenderPassBase() : m_rhi(nullptr), m_viewport()
	{
	}

	RenderPassBase::~RenderPassBase()
	{
		m_rhi.reset();
	}

}