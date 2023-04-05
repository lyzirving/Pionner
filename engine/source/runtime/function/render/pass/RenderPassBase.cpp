#include "function/render/pass/RenderPassBase.h"
#include "function/render/interface/Rhi.h"

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

	void RenderPassBase::initializeUIRenderBackend(WindowUI *ui)
	{
	}

	void RenderPassBase::shutdownUIRenderBackend()
	{
	}

	void RenderPassBase::draw()
	{
	}

	void RenderPassBase::setPassCommonInfo(RenderPassCommonInfo& info)
	{
		m_rhi = info.rhi;
	}

}