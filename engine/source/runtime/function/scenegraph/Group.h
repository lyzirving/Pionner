#ifndef __SCENE_GRAPH_GROUP_H__
#define __SCENE_GRAPH_GROUP_H__

#include <memory>
#include <unordered_map>

#include "SceneGrfObject.h"

namespace pio
{
	namespace scenegrf
	{
		class Node;

		class Group : public std::enable_shared_from_this<Group>, public SceneGrfObject
		{
		public:
			Group();
			~Group();

			void addChild(std::shared_ptr<Node> &node);
			void removeChild(const std::string &name);

		protected:
			typedef std::unordered_map<std::string, std::shared_ptr<Node>> ChildList;

			bool childExist(std::shared_ptr<Node> &node);

		protected:
			ChildList m_children;
		};
	}
}

#endif