#ifndef __PIONNER_SCENEGRAPH_CLEAR_LAYER_H__
#define __PIONNER_SCENEGRAPH_CLEAR_LAYER_H__

#include "Layer.h"

namespace pio
{
	namespace sgf
	{
		class ClearLayer : public Layer
		{
		public:
			ClearLayer();
			virtual ~ClearLayer();

			inline void setColor(const glm::vec4 &color) { m_color = color; }

			virtual void update(RenderInfo &info) override;

		private:
			glm::vec4 m_color{ 0.24f, 0.24f, 0.24f, 1.f };
		};
	}
}

#endif