#include "GeoNode.h"

namespace pio
{
	namespace sgf
	{
		GeoNode::GeoNode() : Group()
		{
			m_type = NODE_TYPE_GEOMETRY_GROUP;
		}

		GeoNode::GeoNode(const std::string &name) : Group(name)
		{
			m_type = NODE_TYPE_GEOMETRY_GROUP;
		}

		GeoNode::~GeoNode()
		{
		}

		template <>
		bool Node::is<GeoNode>()
		{
			return m_type == NODE_TYPE_GEOMETRY_GROUP;
		}

		template <>
		GeoNode *Node::as<GeoNode>()
		{
			if (is<GeoNode>())
			{
				return static_cast<GeoNode *>(this);
			}
			return nullptr;
		}
	}
}