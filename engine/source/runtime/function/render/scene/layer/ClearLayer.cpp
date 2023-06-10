#include "function/render/scene/layer/ClearLayer.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"

#include "function/render/rhi/Rhi.h"

#include "function/render/RenderDef.h"

namespace Pionner
{
	ClearLayer::ClearLayer(const std::shared_ptr<Rhi> &rhi)
		: RenderLayer(rhi)
		, m_color(0.24f, 0.24f, 0.24f)
	{
	}

	ClearLayer::~ClearLayer()
	{
	}

	void ClearLayer::draw(RenderParam &param)
	{
		std::shared_ptr<Rhi> rhi = param.rhi;

		ClearMode clear;
		clear.m_color = glm::vec4(m_color, 1.f);
		clear.m_clearColor = true;
		clear.m_clearDepth = true;

		DepthTest depth;
		depth.m_enbale = true;

		Blend blend = Blend::common();
		CullFace cullFace = CullFace::common();

		rhi->setClearMode(clear);
		rhi->setCullMode(cullFace);
		rhi->setDepthMode(depth);
		rhi->setBlendMode(blend);
	}
}