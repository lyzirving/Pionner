#ifndef __PIONNER_GFX_RENDERER_RESOURCE_RENDERING_DATA_H__
#define __PIONNER_GFX_RENDERER_RESOURCE_RENDERING_DATA_H__

#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/resource/MeshData.h"

namespace pio
{
	class Entity;
	class UniformData;

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

	struct RenderingData
	{
		RenderingData() {}
		~RenderingData() = default;

		RenderingData(const RenderingData &rhs);
		RenderingData(RenderingData &&rhs) noexcept;

		RenderingData &operator=(const RenderingData &rhs);
		RenderingData &operator=(RenderingData &&rhs) noexcept;

		UniformBufferSet UnimBuffSet{};
		std::vector<MeshCmd> OpaqueMeshCmd{};
		std::vector<MeshCmd> TransparentMeshCmd{};
	};
}

#endif