#ifndef __SCENE_GRAPH_GEO_NODE_H__
#define __SCENE_GRAPH_GEO_NODE_H__

#include "Group.h"

namespace pio
{
	namespace sgf
	{
		class GeoNode : public Group
		{
		public:
			GeoNode();
			GeoNode(const std::string &name);
			virtual ~GeoNode();
		};

		template <>
		bool Node::is<GeoNode>();

		template <>
		GeoNode *Node::as<GeoNode>();
	}
}

#endif	