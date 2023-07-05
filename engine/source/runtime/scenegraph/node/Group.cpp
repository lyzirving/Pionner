#include <string.h>

#include "Group.h"
#include "Node.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Group"

namespace pio
{
	namespace sgf
	{
		Group::Group() : Node(), m_children()
		{
			m_type = NODE_TYPE_GROUP;
		}

		Group::Group(const std::string &name) : Node(name), m_children()
		{
			m_type = NODE_TYPE_GROUP;
		}

		Group::~Group()
		{
			m_children.release();
		}

		void Group::addChild(std::shared_ptr<Node> &node)
		{
			if (m_children.itemExist(node->getName()))
				return;

			m_children.add(node->getName(), node);

			auto self = shared_from_this();
			node->addParent(self);
		}

		void Group::addChild(const std::string &parentNodeName, std::shared_ptr<Node> &node)
		{
			if (std::strcmp(parentNodeName.c_str(), m_name.c_str()) == 0)
			{
				addChild(node);
			}
			else
			{
				auto target = findChild(parentNodeName);
				if (!target)
				{
					LOG_ERR("fail to find child node named [%s]", parentNodeName.c_str());
					return;
				}
				Group *pGroup = target->as<Group>();
				if (!pGroup)
				{
					LOG_ERR("child node[%s] is not a group", parentNodeName.c_str());
					return;
				}
				pGroup->addChild(node);
			}
		}

		void Group::removeChild(const std::string &name)
		{
			m_children.remove(name);
		}

		void Group::removeChild(const std::string &parentNodeName, const std::string &name)
		{
			auto parent = findChild(parentNodeName);
			if (!parent)
			{
				LOG_ERR("fail to find target node[%s]", parentNodeName.c_str());
				return;
			}
			Group *pGroup = parent->as<Group>();
			if (!pGroup)
			{
				LOG_ERR("parent node[%s] is not a group", parentNodeName.c_str());
				return;
			}
			pGroup->removeChild(name);
		}

		std::shared_ptr<Node> Group::findChild(const std::string &name)
		{
			// if we find the child in map, the function will directly return
			std::shared_ptr<Node> target = m_children.get(name);

			// if m_children does not contain this child
			if (!target)
			{
				auto itr = m_children.begin();
				auto end = m_children.end();
				while (itr != end)
				{
					auto item = *itr;
					auto pGroup = item->as<Group>();
					if (pGroup)
					{
						target = pGroup->findChild(name);
						if (target)
							break;
					}
				}
			}

			return target;
		}

		template <>
		bool Node::is<Group>() const
		{
			return m_type >= NODE_TYPE_GROUP && m_type < NODE_TYPE_CNT;
		}

		template <>
		Group *Node::as<Group>()
		{
			if (is<Group>())
			{
				return static_cast<Group *>(this);
			}
			return nullptr;
		}
	}
}