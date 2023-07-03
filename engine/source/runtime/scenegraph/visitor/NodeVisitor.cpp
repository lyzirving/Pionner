#include "NodeVisitor.h"

#include "scenegraph/node/Group.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "NodeVisitor"

namespace pio
{
	namespace scenegrf
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

		void NodeVisitor::apply(Group *node)
		{
			apply(static_cast<Node *>(node));
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
