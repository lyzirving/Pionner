#ifndef __SCENE_GRAPH_NODE_VISITOR_H__
#define __SCENE_GRAPH_NODE_VISITOR_H__

#include <memory>

namespace pio
{
	namespace scenegrf
	{
		class Node;

		class NodeVisitor
		{
		public:
			NodeVisitor();
			virtual ~NodeVisitor();

			void apply(const std::shared_ptr<Node> &node);

		protected:
			void traverse();
		};
	}
}

#endif