#include <string.h>

#include "Node.h"
#include "Group.h"
#include "NodeVisitor.h"

namespace pio
{
	namespace scenegrf
	{
		Node::Node() : SceneGrfObject(), m_parents()
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
		}

		void Node::ascend(NodeVisitor *visitor)
		{
			for (size_t i = 0; i < m_parents.size(); i++)
			{
				auto parent = m_parents[i].lock();
				if (parent)
				{
					parent->accept(visitor);
				}
			}
		}

		void Node::traverse(NodeVisitor *visitor)
		{
		}
	}
}