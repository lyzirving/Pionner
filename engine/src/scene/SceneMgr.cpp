#include "SceneMgr.h"

#include "gfx/pipeline/RenderPipeline.h"
#include "physics/PhysicsWorld.h"
#include "animation/AnimationSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SceneMgr"

namespace pio
{
	Ref<Scene> SceneMgr::CreateScene()
	{
		return CreateRef<Scene>(m_Context.lock());
	}

	void SceneMgr::PushBack(const Ref<Scene>& scene)
	{
		if(m_Stack.empty())
		{
			m_Stack.push_back(scene);
		}
		else if(m_Stack.back() != scene)
		{
			m_Stack.push_back(scene);
		}
	}

	Ref<Scene> SceneMgr::PopBack()
	{
		if(m_Stack.empty())
		{
			return Ref<Scene>();
		}
		else
		{
			auto active = m_Stack.back();
			m_Stack.pop_back();
			return active;
		}
	}

	void SceneMgr::Remove(const Ref<Scene>& scene)
	{
		auto it = m_Stack.begin();
		while(it != m_Stack.end())
		{
			if(scene == (*it))
			{
				m_Stack.erase(it);
				return;
			}
			++it;
		}
	}

	void SceneMgr::RemoveAll()
	{
		auto it = m_Stack.begin();
		while(it != m_Stack.end())
		{
			it = m_Stack.erase(it);
		}
	}

	void SceneMgr::OnActive()
	{
		if(m_Active)
		{
			m_Active->OnDetach();
			m_Active.reset();
		}

		if(!m_Stack.empty())
		{
			m_Active = m_Stack.back();
			m_Active->OnAttach();
		}
	}

	void SceneMgr::OnDetach()
	{
		if(m_Active)
		{
			m_Active->OnDetach();
			m_Active.reset();
		}
	}

	void pio::SceneMgr::Tick(const Ref<RenderContext>& context, const Ref<RenderPipeline>& pipeline)
	{
		if(!m_Active)
			return;

		const auto& cameras = m_Active->GetCameras();
		if(cameras.empty())
		{
			LOGE("err! camera must be added into scene");
			return;
		}
		m_Active->GetPhysicsWorld()->Tick(context);
		AnimationSystem::Get()->Tick(context);

		m_Active->Tick(context);
		pipeline->Render(context, cameras);
	}

	void SceneMgr::OnExit()
	{
		RemoveAll();
	}
}