#ifndef __PIONNER_SCENEGRAPH_LAYER_H__
#define __PIONNER_SCENEGRAPH_LAYER_H__

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

			std::shared_ptr<Group> getRootNode() { return m_root; }

		protected:
			LayerType              m_type{ LAYER_TYPE_CNT };
			std::shared_ptr<Group> m_root{ nullptr };
		};
	}
}

#endif