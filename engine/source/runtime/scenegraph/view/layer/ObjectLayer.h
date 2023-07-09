#ifndef __PIONNER_SCENEGRAPH_VIEW_OBJECT_LAYER_H__
#define __PIONNER_SCENEGRAPH_VIEW_OBJECT_LAYER_H__

#include "Layer.h"

namespace pio
{
	namespace sgf
	{
		class ObjectLayer : public Layer
		{
		public:
			ObjectLayer();
			virtual ~ObjectLayer();

			virtual void update(RenderInfo &info) override;
		};
	}
}

#endif