#include <string.h>

#include "Node.h"
#include "Group.h"

#include "scenegraph/visitor/NodeVisitor.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Node"

namespace pio
{
	namespace sgf
	{
		Node::Node() : SceneGrfObject()
		{
		}

		Node::Node(const std::string &name) : SceneGrfObject(name)
		{
		}

		Node::~Node()
		{
			auto itr = m_parents.begin();
			while (itr != m_parents.end())
			{
				(*itr).reset();
				itr = m_parents.erase(itr);
			}

			m_bound.reset();
		}

		void Node::addParent(const std::shared_ptr<Group> &parent)
		{
			if (parent)
				m_parents.push_back(parent);
		}

		void Node::removeParent(const std::string &name)
		{
			auto itr = m_parents.begin();
			while (itr != m_parents.end())
			{
				auto tmp = (*itr).lock();
				if (!tmp)
				{
					itr = m_parents.erase(itr);
					continue;
				}
				if (std::strcmp(name.c_str(), tmp->getName().c_str()) == 0)
				{
					itr = m_parents.erase(itr);
					break;
				}
				itr++;
			}
		}

		void Node::accept(NodeVisitor *visitor)
		{
			if (!visitor)
			{
				LOG_ERR("invalid visitor ptr");
				return;
			}
			visitor->apply(this);
		}

		void Node::ascend(NodeVisitor *visitor)
		{
			if (intercept(visitor))
				return;

			for (size_t i = 0; i < m_parents.size(); i++)
			{
				auto parent = m_parents[i].lock();
				if (parent)
				{
					parent->accept(visitor);
				}
			}
		}

		bool Node::intercept(NodeVisitor *visitor)
		{
			return false;
		}
	}
}