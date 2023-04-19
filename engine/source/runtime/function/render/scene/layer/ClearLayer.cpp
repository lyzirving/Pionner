#include "function/render/scene/layer/ClearLayer.h"
#include "function/render/scene/Camera.h"
#include "function/render/interface/RhiHeader.h"

namespace Pionner
{
	ClearLayer::ClearLayer(const std::shared_ptr<Rhi> &rhi)
		: RenderLayer(rhi)
		, m_color(1.f, 1.f, 1.f)
	{
	}

	ClearLayer::~ClearLayer()
	{
	}

	void ClearLayer::draw(const std::shared_ptr<Camera> &camera)
	{
		glClearColor(m_color.r, m_color.g, m_color.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}