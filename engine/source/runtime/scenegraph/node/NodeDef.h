#ifndef __SCENE_GRAPH_NODE_DEF_H__
#define __SCENE_GRAPH_NODE_DEF_H__

namespace pio
{
	namespace scenegrf
	{
		static const char *ROOT_NODE = "Root";

		enum NodeType
		{
			NODE_TYPE_NORMAL,
			NODE_TYPE_GROUP,
			NODE_TYPE_CNT
		};
	}
}

#endif