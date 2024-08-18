#ifndef __PIONNER_SCENE_SCENE_H__
#define __PIONNER_SCENE_SCENE_H__

#include "asset/Asset.h"
#include "Registry.h"

namespace pio
{
	class RenderContext;
	class RenderPipeline;

	class Scene : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Scene)
	public:
		Scene() : Asset() {}
		~Scene() = default;

		virtual void onAttach();
		virtual void onDetach();

		virtual void onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline);

		Ref<Entity> addEntity(const std::string& name = "")
		{
			auto entity = m_registry.create(name);
			m_ents.push_back(entity);
			return entity;
		}

		template <typename T, typename ... Comps>
		Ref<Entity> addEntity(const std::string& name = "")
		{
			auto entity = m_registry.create<T, Comps...>(name);
			m_ents.push_back(entity);
			return entity;
		}

		void removeEntity(Ref<Entity>& ent);
		void removeAllEntities();

		Registry& registry() { return m_registry; }

	private:
		Registry m_registry{};
		std::vector<Ref<Entity>> m_ents;
	};

	template<>
	bool Asset::is<Scene>() const;
}

#endif