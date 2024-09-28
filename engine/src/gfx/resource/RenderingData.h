#ifndef __PIONNER_GFX_RESOURCE_RENDERING_DATA_H__
#define __PIONNER_GFX_RESOURCE_RENDERING_DATA_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class Node;
	class MeshNode;
	class DirectionalLightNode;
	class SpriteNode;
	class GizmoNode;

	//Node collection that is about to be uploaded
	struct RenderingNodes
	{
		RenderingNodes() {}
		~RenderingNodes() = default;

		RenderingNodes(const RenderingNodes &rhs);
		RenderingNodes(RenderingNodes &&rhs) noexcept;

		RenderingNodes &operator=(const RenderingNodes &rhs);
		RenderingNodes &operator=(RenderingNodes &&rhs) noexcept;

		Ref<DirectionalLightNode> MainLight;
		std::vector<Ref<MeshNode>> Mesh;
		std::vector<Ref<Node>> SkinnedMesh;
		std::vector<Ref<SpriteNode>> Sprite;
		std::vector<Ref<GizmoNode>> Gizmo;
	};

	struct MeshRenderingItem
	{
		RenderingMode Mode{ RenderingMode_Num };
		UUID32 RenderBuffFilter{ InvalidId };
		UUID32 MaterialFilter{ InvalidId };

		MeshRenderingItem() {}
		MeshRenderingItem(const MeshRenderingItem& rhs);
		MeshRenderingItem(MeshRenderingItem&& rhs) noexcept;

		MeshRenderingItem& operator=(const MeshRenderingItem& rhs);
		MeshRenderingItem& operator=(MeshRenderingItem&& rhs) noexcept;
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

		void submitMesh(MeshRenderingItem&& item);
		void submitSprite(MeshRenderingItem&& item);
		void submitOutline(MeshRenderingItem&& item);

		std::map<uint8_t, UUID32> UnimBuffSet{};
		std::vector<MeshRenderingItem> OpaqueMeshItems{};
		std::vector<MeshRenderingItem> TransparentMeshItems{};
		std::vector<MeshRenderingItem> UiSprites{};
		std::vector<MeshRenderingItem> Outlines{};
	};
}

#endif