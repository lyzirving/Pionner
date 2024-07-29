#ifndef __PIONNER_SCENE_SCENE_H__
#define __PIONNER_SCENE_SCENE_H__

#include "core/EventBus.h"
#include "physics/PhysicsDef.h"
#include "ui/UiDef.h"

#include "Registry.h"
#include "Components.h"
#include "Light.h"

namespace pio
{
	class SceneRenderer;
	class Skybox;
	struct LayoutViewport;

	/*
	* Scene is somewhere used to store logical data
	*/
	class Scene : public EventBusObject
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void onAttach(Ref<SceneRenderer> &renderer);
		virtual void onDetach(Ref<SceneRenderer> &renderer);

		// this method is used to update logic, which should be called before onRender
		virtual void onUpdate(const Timestep &ts);
		// update rendering
		virtual void onRender(Ref<SceneRenderer> &renderer, const Timestep &ts);

		void setCameraViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void setLayoutParam(const WindowLayoutParams &p) { m_layoutParam = p; }

	private:
		void createData();
		void simulate(const Timestep &ts);

		void onAssetUnzip(const EventBusArg &arg);

	private:
		static Registry *s_registry;

	private:
		WindowLayoutParams m_layoutParam;

		LightEnvironment m_lightEnv;
		Ref<Skybox> m_skybox;

		Ref<Entity> m_sceneRoot{ nullptr };			
		AssetHandle m_screenQuad{ NullAsset };

	private:
		friend class SceneRenderer;
	};
}

#endif