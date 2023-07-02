#ifndef __SCENE_GRAPH_NODE_H__
#define __SCENE_GRAPH_NODE_H__

#include <vector>
#include <memory>

#include "SceneGrfObject.h"

namespace pio
{
	namespace scenegrf
	{
		class Group;
		class NodeVisitor;

		class Node : public SceneGrfObject
		{
			friend class Group;
			friend class NodeVisitor;

		public:
			Node();
			virtual ~Node();
			
		protected:
			typedef std::vector<std::weak_ptr<Group>> ParentList;

			void addParent(const std::shared_ptr<Group> &parent);
			void removeParent(const std::string &name);

			void ascend(NodeVisitor *visitor);
			void descend(NodeVisitor *visitor);

		protected:
			ParentList  m_parents;
		};
	}
}

#endif