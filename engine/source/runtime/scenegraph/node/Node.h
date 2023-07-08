#ifndef __SCENE_GRAPH_NODE_H__
#define __SCENE_GRAPH_NODE_H__

#include <vector>
#include <memory>

#include "scenegraph/SceneGrfDef.h"
#include "scenegraph/SceneGrfObject.h"

#include "gfx/shape/Shape.h"

namespace pio
{
	class PioEntity;

	namespace sgf
	{
		class Group;
		class NodeVisitor;

		class Node : public SceneGrfObject
		{
			friend class Group;
			friend class NodeVisitor;

		public:
			Node();
			Node(const std::string &name);
			virtual ~Node();

			/**
			* Entry to traverse the node's hierarchy.
			*/
			virtual void accept(NodeVisitor *visitor);

			virtual void ascend(NodeVisitor *visitor);
			virtual void descend(NodeVisitor *visitor) {}

			virtual void swap(const PioEntity &entity) {}
			virtual void release() {}

			void setBound(const std::shared_ptr<gfx::Shape> &shape) { m_bound = shape; }

			template <class T>
			bool is();

			template <class T>
			T *as();
			
		protected:
			virtual bool intercept(NodeVisitor *visitor);

			typedef std::vector<std::weak_ptr<Group>> ParentList;

			void addParent(const std::shared_ptr<Group> &parent);
			void removeParent(const std::string &name);

		protected:
			ParentList                  m_parents{};
			NodeType                    m_type{ NODE_TYPE_CNT };
			std::shared_ptr<gfx::Shape> m_bound{ nullptr };
		};

		template <class T>
		bool Node::is() { return false; }

		template <class T>
		T *Node::as() { return nullptr; }
	}
}

#endif