#ifndef __SCENE_GRAPH_NODE_VISITOR_H__
#define __SCENE_GRAPH_NODE_VISITOR_H__

#include <memory>

namespace pio
{
	namespace sgf
	{
		class Node;
		class Group;
		class GeoNode;
		class Geometry;
		class Drawable;

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

			// Sub class should implement this method to do its logic.
			virtual void apply(Node *node);

			void apply(Drawable *node);
			void apply(Geometry *node);
			void apply(Group *node);
			void apply(GeoNode *node);

		protected:
			void traverse(Node *node);

		protected:
			TraversalMode m_traversalMode;
		};
	}
}

#endif