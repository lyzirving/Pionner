#ifndef __SCENE_GRAPH_GROUP_H__
#define __SCENE_GRAPH_GROUP_H__

#include <memory>
#include <unordered_map>

#include "core/struct/LinkedMap.h"

#include "Node.h"

namespace pio
{
	namespace sgf
	{
		class Group : public Node, public std::enable_shared_from_this<Group>
		{
		public:
			Group();
			Group(const std::string &name);
			virtual ~Group();

			virtual void descend(NodeVisitor *visitor) override;

			void addChild(std::shared_ptr<Node> &node);
			/**
			* @param parentNodeName: target node's name
			* @param node:           child node that needs to be added 
			*/
			void addChild(const std::string &parentNodeName, std::shared_ptr<Node> &node);
			/**
			* @brief: remove child node in current child list.
			*/
			void removeChild(const std::string &name);
			/**
			* @brief: remove child node in target node's list.
			*/
			void removeChild(const std::string &parentNodeName, const std::string &name);

			inline uint32_t childSize() { return m_children.size(); }

		protected:
			/**
			* @brief find named child node in depth-first order.
			*/
			std::shared_ptr<Node> findChild(const std::string &name);

		protected:
			core::LinkedMap<std::string, Node> m_children;
		};

		template <>
		bool Node::is<Group>();

		template <>
		Group *Node::as<Group>();

	}
}

#endif