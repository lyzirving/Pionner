#include "Group.h"
#include "Node.h"

namespace pio
{
	namespace scenegrf
	{
		Group::Group() : Node(), m_children()
		{
		}

		Group::~Group()
		{
			auto itr = m_children.begin();
			while (itr != m_children.end())
			{
				itr->second.reset();
				itr = m_children.erase(itr);
			}
		}

		void Group::addChild(std::shared_ptr<Node> &node)
		{
			if (childExist(node))
				return;

			m_children.insert(std::make_pair(node->getName(), node));

			auto self = shared_from_this();
			node->addParent(self);
		}

		void Group::removeChild(const std::string &name)
		{
			auto itr = m_children.find(name);
			if (itr != m_children.end())
			{
				itr->second->removeParent(m_name);
				itr->second.reset();
				m_children.erase(itr);
			}
		}

		bool Group::childExist(std::shared_ptr<Node> &node)
		{
			if(!node)
				return false;

			auto itr = m_children.find(node->getName());
			return itr != m_children.end();
		}
	}
}