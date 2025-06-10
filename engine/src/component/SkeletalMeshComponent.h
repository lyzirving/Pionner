#pragma once

#include "Component.h"

namespace pio
{
	class Material;
	class SkinnedMesh;
	class UniformBlock;
	class UniformBuffer;
	class MeshRenderBuffer;
	class TransformComponent;
	class RenderingData;
	class AnimationState;
	class AnimationClip;

	class SkeletalMeshComponent : public Component
	{
		RTTR_ENABLE(Component)
	public:
		SkeletalMeshComponent();
		virtual ~SkeletalMeshComponent() = default;
		virtual void OnTick();

		void SetTransformComponent(const Ref<TransformComponent>& comp);		
		void OnBoneTransformChange();
		void OnRender(RenderingData& data);
		
		// -------------- AnimationState Interface ----------------		
		void Start();
		void Stop();
		void SetStartTime(uint64_t time);
		void SetInterval(uint64_t time);
		void SetLoop(bool loop);
		void SetAnimationClip(uint32_t index);
		void SetAnimationClip(const Ref<AnimationClip>& clip);
		void RemoveAnimationClip();
		void SetLocalTimePos(float pos);
		void SetBlendWeight(float weight);

		bool IsAnimSet() const;
		bool IsStart() const;
		bool IsLoop() const;

		float GetLocalTimePos() const;
		float GetBlendWeight() const;
		uint64_t GetStartTime() const;
		uint64_t GetInterval() const;
		float GetDuration() const;
		uint32_t GetTickNum() const;
		Ref<AnimationClip> GetAnimClip() const;
		// --------------------------------------------------------
		const std::vector<Ref<UniformBuffer>>& GetMotionBuffer() const { return m_MotionBuffers; }

		std::vector<glm::mat4>& GetBoneTransform() { return m_BoneTransforms; }
		const std::vector<glm::mat4>& GetBoneTransform() const { return m_BoneTransforms; }

		std::vector<Ref<Material>>& GetMaterialElems() { return m_MaterialElems; }
		const std::vector<Ref<Material>>& GetMaterialElems() const { return m_MaterialElems; }

	protected:
		void CreateBuffer();
		void CreateSkinningBuffer();
		void OnTransformChange();

		PIO_DEFINE_PROPERTY(Ref<SkinnedMesh>, Mesh)
		PIO_DEFINE_PROPERTY(bool, bCastShadow, true)
		PIO_DEFINE_PROPERTY(bool, AnimRegisterd, false)

	private:
		std::vector<Ref<MeshRenderBuffer>> m_RenderBuff;
		std::vector<Ref<UniformBuffer>> m_MotionBuffers;
		std::vector<Ref<Material>> m_MaterialElems;

		Ref<AnimationState> m_AnimationState;
		std::vector<glm::mat4> m_BoneTransforms;
		std::vector<glm::mat4> m_ReferenceBoneTransforms;

		Ref<UniformBlock> m_SkinningUBlock;
		Ref<UniformBuffer> m_SkinningUBuffer;

		Ref<TransformComponent> m_TransComp;
	};
}