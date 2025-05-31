#pragma once

#include "Node.h"

namespace pio
{
	class SkinnedMesh;

	class SkinnedMeshNode : public Node
	{
		RTTR_ENABLE(Node)
	public:
		SkinnedMeshNode();
		virtual ~SkinnedMeshNode() = default;

		void SetMesh(const Ref<SkinnedMesh>& mesh);
		void SetAnimationClip(uint32_t index);
		void StartAnimation();
		void StopAnimation();
		void SetLoop(bool loop);

	protected:
		virtual void OnInit() override;
		virtual void OnBeginFrameTick(const Ref<RenderContext>& context) override;
		virtual void OnTickComponents(const Ref<RenderContext>& context) override;
		virtual void OnTick(const Ref<RenderContext>& context) override;
		virtual void OnRender(const Ref<RenderContext>& context) override;

	protected:
		AABB m_BoundingBox;

	private:
		friend class SkeletalMeshComponent;
	};
}