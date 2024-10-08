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
	/*
	* Scene is somewhere used to store logical data
	*/
	class Scene : public EventBusObject, public Asset, public std::enable_shared_from_this<Scene>
	{
		OVERRIDE_ASSET_TYPE(AssetType::Scene)
	public:
		static Ref<Scene>  Main;
		static Ref<Entity> Root;

	public:
		Scene(bool bMain = false);
		virtual ~Scene();

		virtual void onAttach(Ref<SceneRenderer>& renderer, const LayoutParams &param);
		virtual void onDetach(Ref<SceneRenderer> &renderer);

		// this method is used to update logic, which should be called before onRender
		virtual void onUpdate(const Timestep &ts);
		// update rendering
		virtual void onRender(Ref<SceneRenderer> &renderer, const Timestep &ts);

		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void setSimulate(bool enable) { m_bSimulate = enable; }

		bool bSimulate() const { return m_bSimulate; }
		Ref<PhysicsScene> physics() const { return m_physics; }
		Ref<Camera> camera() const { return m_camera; }

		Ref<Scene> self() { return shared_from_this(); }
	private:
		void createData();
		void simulate(const Timestep &ts);

		void onAssetUnzip(const EventBusArg &arg);

	private:
		static Registry *s_registry;

	private:
		LightEnvironment m_lightEnv;

		bool m_bMain{ false };
		bool m_bSimulate{ false };
		Ref<PhysicsScene> m_physics;

		Ref<Camera> m_camera;

		Ref<Entity> m_sceneRoot{ nullptr };			
		AssetHandle m_screenQuad{ NullAsset };

	private:
		friend class SceneRenderer;
	};

	template<>
	bool Asset::is<Scene>() const;
}

#endif