#pragma once

#include "Scene.h"

namespace pio
{
	class RenderPipeline;

	class SceneMgr
	{
	public:
		SceneMgr(const Ref<RenderContext>& context) : m_Context(context) {}
		~SceneMgr() = default;

		Ref<Scene> CreateScene();

		void PushBack(const Ref<Scene>& scene);
		Ref<Scene> PopBack();

		void Remove(const Ref<Scene>& scene);
		void RemoveAll();

		void OnActive();
		void OnDetach();
		void Tick(const Ref<RenderContext>& context, const Ref<RenderPipeline>& pipeline);
		void OnExit();

		const Ref<Scene>& ActiveScene() const { return m_Active; }

	private:
		WeakRef<RenderContext> m_Context;
		std::list<Ref<Scene>> m_Stack;
		Ref<Scene> m_Active;
	};
}