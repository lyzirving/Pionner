#include "NodeVisitor.h"

#include "scenegraph/node/GeoNode.h"
#include "scenegraph/node/drawable/Geometry.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "NodeVisitor"

namespace pio
{
	namespace sgf
	{
		NodeVisitor::NodeVisitor() : m_traversalMode(TRAVERSE_NONE)
		{
		}

		NodeVisitor::NodeVisitor(TraversalMode mode) : m_traversalMode(mode)
		{
		}

		NodeVisitor::~NodeVisitor() = default;

		void NodeVisitor::apply(Node *node)
		{
			traverse(node);
		}

		void NodeVisitor::apply(Drawable *node)
		{
			if (!node)
				return;

			apply(static_cast<Node *>(node));
		}

		void NodeVisitor::apply(Geometry *node)
		{
			if (!node)
				return;

			apply(static_cast<Drawable *>(node));
		}

		void NodeVisitor::apply(Group *node)
		{
			if (!node)
				return;

			apply(static_cast<Node *>(node));
		}

		void NodeVisitor::apply(GeoNode *node)
		{
			if (!node)
				return;

			apply(static_cast<Group *>(node));
		}

		void NodeVisitor::traverse(Node *node)
		{
			if (!node)
			{
				LOG_ERR("invalid input node");
				return;
			}

			if (m_traversalMode == TRAVERSE_PARENTS)
			{
				node->ascend(this);
			}
			else
			{
				node->descend(this);
			}
		}
	}
}
