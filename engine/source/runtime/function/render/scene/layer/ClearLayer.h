#ifndef __RENDER_CLEAR_LAYER_H__
#define __RENDER_CLEAR_LAYER_H__

#include <glm/glm.hpp>

#include "function/render/scene/layer/RenderLayer.h"

namespace Pionner
{
	class ClearLayer : public RenderLayer
	{
	public:
		ClearLayer(const std::shared_ptr<Rhi> &rhi);
		virtual ~ClearLayer();

		inline void setColor(const glm::vec3 &color) { m_color = color; }

		virtual void draw(RenderParam &param) override;

	private:
		glm::vec3 m_color;
	};
}

#endif