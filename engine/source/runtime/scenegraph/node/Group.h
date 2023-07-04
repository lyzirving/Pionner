#ifndef __SCENE_GRAPH_GROUP_H__
#define __SCENE_GRAPH_GROUP_H__

#include <memory>
#include <unordered_map>

#include "core/struc/LinkedMap.h"

#include "Node.h"

namespace pio
{
	namespace scenegrf
	{
		class Group : public Node, public std::enable_shared_from_this<Group>
		{
		public:
			Group();
			Group(const std::string &name);
			~Group();

			void addChild(std::shared_ptr<Node> &node);
			void addChild(const std::string &nodeName, std::shared_ptr<Node> &node);
			void removeChild(const std::string &name);

		protected:
			typedef std::unordered_map<std::string, std::shared_ptr<Node>> ChildList;

			bool childExist(std::shared_ptr<Node> &node);

		protected:
			ChildList m_children;
			core::LinkedMap<std::string, Node> m_childs;
		};
	}
}

#endif