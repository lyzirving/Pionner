#ifndef __SCENE_GRAPH_DEF_H__
#define __SCENE_GRAPH_DEF_H__

#include <glm/glm.hpp>

namespace pio
{
	namespace sgf
	{
		static const char *ROOT_NODE = "Root";

		enum NodeType
		{
			NODE_TYPE_NORMAL,
			// Types bellow are referenced to a group
			NODE_TYPE_GROUP,
			// Type count
			NODE_TYPE_CNT
		};

		struct RenderInfo
		{
		};

		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};
	}
}

#endif	