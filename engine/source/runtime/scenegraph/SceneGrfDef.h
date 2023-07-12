#ifndef __SCENE_GRAPH_DEF_H__
#define __SCENE_GRAPH_DEF_H__

#include <memory>
#include <glm/glm.hpp>

namespace pio
{
	namespace gfx
	{
		class GraphicContext;
	}

	namespace sgf
	{
		static const char *ROOT_NODE = "Root";

		enum NodeType
		{
			NODE_TYPE_NORMAL,
			NODE_TYPE_GEOMETRY,
			// Types bellow are referenced to a group
			NODE_TYPE_GROUP,
			NODE_TYPE_GEOMETRY_GROUP,
			// Type count
			NODE_TYPE_CNT
		};

		enum LayerType : uint8_t
		{
			LAYER_TYPE_OBJ,
			LAYER_TYPE_CNT
		};

		enum CameraId : uint8_t
		{
			CAM_ID_MAIN,
			CAM_ID_CNT
		};

		struct Viewport
		{
			int32_t x{ 0 }, y{ 0 };
			int32_t width{ 0 }, height{ 0 };
		};

		struct RenderInfo
		{
			std::shared_ptr<gfx::GraphicContext> gfxContext{ nullptr };
			uint64_t  deltaMs{ 0 };
			glm::mat4 viewMat{ 1.f };
			glm::mat4 prjMat { 1.f };
			Viewport  viewport{};
		};

		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 texCoord;
			glm::vec3 tangents;
		};
	}
}

#endif	