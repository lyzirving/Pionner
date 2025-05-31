#include "AnimationSystem.h"
#include "AnimationPipeline.h"

#include "component/SkeletalMeshComponent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AnimationSystem"

namespace pio
{
	PIO_SINGLETON_IMPL(AnimationSystem)

	void AnimationSystem::Init(const InitParam& param)
	{
		AnimationSystem::Get();
	}

	void AnimationSystem::Shutdown()
	{
		AnimationSystem::Get()->Release();
	}

	void AnimationSystem::Tick(const Ref<RenderContext>& context)
	{
		AnimationPipeline::Tick(context, m_SkeletalComps);		
	}

	void AnimationSystem::Release()
	{
		m_SkeletalComps.clear();
	}

	void AnimationSystem::Inject(const Ref<SkeletalMeshComponent>& comp)
	{
		if(!comp)
			return;

		if(m_SkeletalComps.find(comp->UId()) == m_SkeletalComps.end())
		{
			m_SkeletalComps.insert({ comp->UId(), comp });
		}
	}

	void AnimationSystem::Remove(const Ref<SkeletalMeshComponent>& comp)
	{
		if(!comp)
			return;

		m_SkeletalComps.erase(comp->UId());
	}
}