#ifndef __PIONNER_SCENEGRAPH_LAYER_H__
#define __PIONNER_SCENEGRAPH_LAYER_H__

#include "scenegraph/SceneGrfDef.h"

namespace pio
{
	namespace sgf
	{
		class Layer
		{
		public:
			Layer() {};
			Layer(LayerType type) : m_type(type) {};

			virtual ~Layer() = default;
			virtual void update(RenderInfo &info) = 0;

		protected:
			LayerType m_type{ LAYER_TYPE_CNT };
		};
	}
}

#endif