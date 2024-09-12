#ifndef __PIONNER_SCENE_SCENE_H__
#define __PIONNER_SCENE_SCENE_H__

#include "asset/Asset.h"
#include "Registry.h"

namespace pio
{
	class Camera;
	class RenderContext;
	class RenderPipeline;

	class Scene : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::SceneResource)
	public:
		Scene() : Asset() {}
		~Scene() = default;

		virtual void onAttach();
		virtual void onDetach();

		virtual void onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline, std::vector<Ref<Camera>>& cameras);

		Ref<Entity> addEntity(EntityType type, const std::string& name = "")
		{
			auto entity = m_registry.create(type, name);
			m_ents.push_back(entity);
			return entity;
		}

		template <typename T, typename ... Comps>
		Ref<Entity> addEntity(EntityType type, const std::string& name = "")
		{
			auto entity = m_registry.create<T, Comps...>(type, name);
			m_ents.push_back(entity);
			return entity;
		}
		
		void setMainCamera(const Ref<Camera>& cam) { if (m_mainCamera != cam) { m_mainCamera = cam; } }

		void removeEntity(Ref<Entity>& ent);
		void removeAllEntities();

		Registry& registry() { return m_registry; }

		const std::vector<Ref<Entity>>& entities() const { return m_ents; }
		const Ref<Camera>& mainCamera() const { return m_mainCamera; }

	private:
		Registry m_registry{};
		std::vector<Ref<Entity>> m_ents;
		Ref<Camera> m_mainCamera;
	};

	template<>
	bool Asset::is<Scene>() const;
}

#endif