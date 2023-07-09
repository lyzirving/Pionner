#ifndef __PIONNER_SCENEGRAPH_VIEW_LAYER_H__
#define __PIONNER_SCENEGRAPH_VIEW_LAYER_H__

#include <memory>

#include "scenegraph/SceneGrfDef.h"
#include "scenegraph/node/Group.h"

namespace pio
{
	namespace sgf
	{
		class Layer
		{
		public:
			Layer();
			Layer(LayerType type);

			virtual ~Layer();

			virtual void update(RenderInfo &info) = 0;
			virtual void release() {}

			void addNode(std::shared_ptr<Node> &node);
			void addNode(const std::string &parentNodeName, std::shared_ptr<Node> &node);

			inline std::shared_ptr<Group> getRootNode() { return m_root; }
			inline LayerType              getLayerType() { return m_type; }

		protected:
			LayerType              m_type{ LAYER_TYPE_CNT };
			std::shared_ptr<Group> m_root{ nullptr };
		};
	}
}

#endif