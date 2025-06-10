#pragma once

#include "Component.h"

namespace pio
{	
	class Material;
	class StaticMesh;
	class UniformBuffer;
	class MeshRenderBuffer;
	class TransformComponent;	
	class RenderingData;
	struct SubMesh;

	class StaticMeshComponent : public Component
	{
		RTTR_ENABLE(Component)
	public:
		StaticMeshComponent() : Component() {};
		virtual ~StaticMeshComponent() = default;
		virtual void OnTick() override;

		void SetSubMesh(int32_t index, const SubMesh& mesh);
		void SetTransformComponent(const Ref<TransformComponent>& comp);		
		void OnRender(RenderingData& data);		

		const std::vector<Ref<MeshRenderBuffer>>& GetRenderBuffer() const { return m_RenderBuff; }
		const std::vector<Ref<UniformBuffer>>& GetMotionBuffer() const { return m_MotionBuffers; }

		std::vector<Ref<Material>>& GetMaterialElems() { return m_MaterialElems; }
		const std::vector<Ref<Material>>& GetMaterialElems() const { return m_MaterialElems; }

		PIO_DEFINE_PROPERTY(Ref<StaticMesh>, Mesh)
		PIO_DEFINE_PROPERTY(bool, bCastShadow, true)

	protected:
		void OnTransformChange();

	private:
		void CreateBuffer();
		void UpdateIndexedBuffer(int32_t index);

	private:
		std::vector<Ref<MeshRenderBuffer>> m_RenderBuff;		
		std::vector<Ref<UniformBuffer>> m_MotionBuffers;
		std::vector<Ref<Material>> m_MaterialElems;
		Ref<TransformComponent> m_TransComp;
	};
}