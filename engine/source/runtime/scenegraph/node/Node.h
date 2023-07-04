#ifndef __SCENE_GRAPH_NODE_H__
#define __SCENE_GRAPH_NODE_H__

#include <vector>
#include <memory>

#include "scenegraph/SceneGrfObject.h"

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

			/**
			* Entry to traverse the node's hierarchy.
			*/
			virtual void accept(NodeVisitor *visitor);

			virtual void ascend(NodeVisitor *visitor);
			virtual void descend(NodeVisitor *visitor) {}

			virtual void release() {}
			
		protected:
			virtual bool intercept(NodeVisitor *visitor);

			typedef std::vector<std::weak_ptr<Group>> ParentList;

			void addParent(const std::shared_ptr<Group> &parent);
			void removeParent(const std::string &name);

		protected:
			ParentList  m_parents;
		};
	}
}

#endif