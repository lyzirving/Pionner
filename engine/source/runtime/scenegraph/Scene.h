#ifndef __PIONNER_SCENEGRAPH_SCENE_H__
#define __PIONNER_SCENEGRAPH_SCENE_H__

#include <memory>

#include "SceneGrfDef.h"

namespace pio
{
	namespace sgf
	{
		class Layer;

		class Scene
		{
		public:
			Scene();
			~Scene();

			void update(RenderInfo &info);

		private:
			void createLayers();

		private:
			std::shared_ptr<Layer> m_layers[LAYER_TYPE_CNT];
		};
	}
}

#endif