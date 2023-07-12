#ifndef __PIONNER_SCENEGRAPH_VIEW_SCENE_H__
#define __PIONNER_SCENEGRAPH_VIEW_SCENE_H__

#include <memory>
#include <vector>
#include <string>

#include "scenegraph/SceneGrfDef.h"

namespace pio
{
	namespace sgf
	{
		class Layer;
		class Node;
		class Camera;

		class Scene
		{
		public:
			Scene();
			virtual ~Scene();

			virtual void addNode(std::shared_ptr<Node> &node);
			virtual void addNode(const std::string &parentNodeName, std::shared_ptr<Node> &node);
			virtual void update(RenderInfo &info);
			virtual void release();

			void setWndSize(int32_t width, int32_t height);
			void tick(uint64_t deltaMs);
			
			inline void sortLayer() { m_sortLayer = true; }

		protected:
			static bool layerSorter(const std::shared_ptr<Layer> &lhs, const std::shared_ptr<Layer> &rhs);

			virtual void createCameras();
			virtual void createLayers();

		private:
			std::vector<std::shared_ptr<Camera>> m_cameras{};
			std::vector<std::shared_ptr<Layer>>  m_layers{};
			bool                                 m_sortLayer{ true };
		};
	}
}

#endif