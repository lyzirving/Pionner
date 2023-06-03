#include "function/render/pass/RenderPassBase.h"
#include "function/render/rhi/Rhi.h"

#include "function/ui/WindowUI.h"

namespace Pionner
{
	RenderPassBase::RenderPassBase() : m_rhi(nullptr)
	{
	}

	RenderPassBase::~RenderPassBase()
	{
		m_rhi.reset();
	}

}