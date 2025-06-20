#include "gfx/GfxDef.h"

namespace pio
{
	class Material;
	class MeshRenderBuffer;
	class UniformBuffer;
	class UniformData;
	class FrameBuffer;

	struct MeshRenderingItem
	{
		RenderingMode Mode{ RenderingMode_Num };
		DrawMode DrawFlag{ DrawMode_Triangle };
		Ref<MeshRenderBuffer> MeshBuff;
		Ref<Material> Material;
		std::vector<Ref<UniformBuffer>> UniformBuffers;
		glm::vec3 Center{ 0.f };

		MeshRenderingItem() {}
		MeshRenderingItem(const MeshRenderingItem& rhs);
		MeshRenderingItem(MeshRenderingItem&& rhs) noexcept;

		MeshRenderingItem& operator=(const MeshRenderingItem& rhs);
		MeshRenderingItem& operator=(MeshRenderingItem&& rhs) noexcept;
	};

	struct PipelineData
	{
		PipelineData() {}
		~PipelineData() = default;

		PipelineData(const PipelineData& rhs);
		PipelineData(PipelineData&& rhs) noexcept;

		PipelineData& operator=(const PipelineData& rhs);
		PipelineData& operator=(PipelineData&& rhs) noexcept;

		Ref<FrameBuffer> DirLitShadowMap;
		Ref<FrameBuffer> IndirectLight;
		Ref<FrameBuffer> PointLitShadowMap;
		Ref<FrameBuffer> GBuffer;
		Ref<FrameBuffer> DefferedBuffer;
	};

	struct RenderingData
	{
		RenderingData() {}
		~RenderingData() = default;

		RenderingData(const RenderingData& rhs);
		RenderingData(RenderingData&& rhs) noexcept;

		RenderingData& operator=(const RenderingData& rhs);
		RenderingData& operator=(RenderingData&& rhs) noexcept;

		Ref<UniformBuffer> UBuffs[UBBinding_Num];
		std::vector<MeshRenderingItem> OpaqueMeshItems{};
		std::vector<MeshRenderingItem> TransparentMeshItems{};
		std::vector<MeshRenderingItem> ShadowCasterItems{};
		std::vector<MeshRenderingItem> WireframeItems{};
		std::vector<MeshRenderingItem> SpriteItems{};
		PipelineData Pipeline;
		uint32_t PermutationFlags{ 0 };
	};
}