#pragma once

#include "Common.h"

namespace pio
{
	class SkeletalMeshComponent;	

	class AnimationSystem
	{
		PIO_SINGLETON_DECLARE(AnimationSystem)

	public:
		void Tick(const Ref<RenderContext>& context);
		void Release();

		void Inject(const Ref<SkeletalMeshComponent>& comp);
		void Remove(const Ref<SkeletalMeshComponent>& comp);

	private:
		std::map<uint32_t, WeakRef<SkeletalMeshComponent>> m_SkeletalComps;
	};
}