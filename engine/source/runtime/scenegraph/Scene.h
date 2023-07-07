#ifndef __PIONNER_SCENEGRAPH_SCENE_H__
#define __PIONNER_SCENEGRAPH_SCENE_H__

#include <memory>
#include <string>

#include "SceneGrfDef.h"

namespace pio
{
	namespace sgf
	{
		class Layer;
		class Node;

		class Scene
		{
		public:
			Scene();
			virtual ~Scene();

			virtual void addNode(std::shared_ptr<Node> &node);
			virtual void addNode(const std::string &parentNodeName, std::shared_ptr<Node> &node);
			virtual void update(RenderInfo &info);
			virtual void release();

		protected:
			virtual void createLayers();

		private:
			std::shared_ptr<Layer> m_layers[LAYER_TYPE_CNT];
		};
	}
}

#endif