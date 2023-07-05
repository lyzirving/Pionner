#ifndef __SCENE_GRAPH_NODE_FACTORY_H__
#define __SCENE_GRAPH_NODE_FACTORY_H__

#include <memory>

#include "global/PioEntity.h"

namespace pio
{
	namespace sgf
	{
		class Node;

		class NodeFactory
		{
		public:
			static std::shared_ptr<Node> create(PioEntityType type);
		private:
			NodeFactory() {}
			~NodeFactory() = default;
		};
	}
}

#endif