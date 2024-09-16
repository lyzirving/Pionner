#ifndef __PIONNER_GFX_RESOURCE_RENDERING_DATA_H__
#define __PIONNER_GFX_RESOURCE_RENDERING_DATA_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class Node;
	class MeshNode;

	//Node collection that is about to be uploaded
	struct RenderingNodes
	{
		RenderingNodes() {}
		~RenderingNodes() = default;

		RenderingNodes(const RenderingNodes &rhs);
		RenderingNodes(RenderingNodes &&rhs) noexcept;

		RenderingNodes &operator=(const RenderingNodes &rhs);
		RenderingNodes &operator=(RenderingNodes &&rhs) noexcept;

		Ref<Node> MainLight;
		std::vector<Ref<MeshNode>> Mesh;
		std::vector<Ref<Node>> SkinnedMesh;
	};

	struct MeshRenderingItem
	{
		RenderingMode Mode{ RenderingMode_Opaque };
		UUID32 RenderBuffFilter{ InvalidId };
		UUID32 MaterialFilter{ InvalidId };
	};

	/*
	* @brief: RenderingData is transient data in rendering pipeline, so you'd better use id to search for its member filed
	*/
	struct RenderingData
	{
		RenderingData() {}
		~RenderingData() = default;

		RenderingData(const RenderingData &rhs);
		RenderingData(RenderingData &&rhs) noexcept;

		RenderingData &operator=(const RenderingData &rhs);
		RenderingData &operator=(RenderingData &&rhs) noexcept;

		std::map<uint8_t, UUID32> UnimBuffSet{};
		std::vector<MeshRenderingItem> OpaqueMeshItems{};
		std::vector<MeshRenderingItem> TransparentMeshItems{};
	};
}

#endif