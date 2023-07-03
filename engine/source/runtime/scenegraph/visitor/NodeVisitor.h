#ifndef __SCENE_GRAPH_NODE_VISITOR_H__
#define __SCENE_GRAPH_NODE_VISITOR_H__

#include <memory>

namespace pio
{
	namespace scenegrf
	{
		class Node;
		class Group;

		class NodeVisitor
		{
		public:
			enum TraversalMode : uint8_t
			{
				TRAVERSE_NONE,
				TRAVERSE_PARENTS,
				TRAVERSE_ALL_CHILDREN,
				TRAVERSE_ACTIVE_CHILDREN
			};

		public:
			NodeVisitor();
			NodeVisitor(TraversalMode mode);
			virtual ~NodeVisitor();

			void apply(Node  *node);
			void apply(Group *node);

		protected:
			void traverse(Node *node);

		protected:
			TraversalMode m_traversalMode;
		};
	}
}

#endif