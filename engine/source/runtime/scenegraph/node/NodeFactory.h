#ifndef __SCENE_GRAPH_NODE_FACTORY_H__
#define __SCENE_GRAPH_NODE_FACTORY_H__

#include <memory>

#include "world/entity/PioEntityDef.h"

namespace pio
{
	namespace scenegrf
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