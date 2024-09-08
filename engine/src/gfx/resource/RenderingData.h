#ifndef __PIONNER_GFX_RESOURCE_RENDERING_DATA_H__
#define __PIONNER_GFX_RESOURCE_RENDERING_DATA_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class Entity;

	//Entity collection that is about to be uploaded
	struct RenderingEntities
	{
		RenderingEntities() {}
		~RenderingEntities() = default;

		RenderingEntities(const RenderingEntities &rhs);
		RenderingEntities(RenderingEntities &&rhs) noexcept;

		RenderingEntities &operator=(const RenderingEntities &rhs);
		RenderingEntities &operator=(RenderingEntities &&rhs) noexcept;

		Ref<Entity> MainLight;
		std::list<Ref<Entity>> Mesh;
		std::list<Ref<Entity>> SkinnedMesh;
	};

	struct MeshRenderingItem
	{
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