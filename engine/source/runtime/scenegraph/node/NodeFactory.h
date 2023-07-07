#ifndef __SCENE_GRAPH_NODE_FACTORY_H__
#define __SCENE_GRAPH_NODE_FACTORY_H__

#include <memory>

#include "global/GlobalDef.h"

namespace pio
{
	namespace sgf
	{
		class Node;

		class NodeFactory
		{
		public:
			static std::shared_ptr<Node> create(const EntityParam &param);
		private:
			NodeFactory() {}
			~NodeFactory() = default;
		};
	}
}

#endif