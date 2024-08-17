#ifndef __PIONNER_SCENE_SCENE_H__
#define __PIONNER_SCENE_SCENE_H__

#include "asset/Asset.h"

namespace pio
{
	class RenderContext;
	class Entity;

	class Scene : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Scene)
	public:
		Scene() : Asset() {}
		~Scene() = default;

		virtual void onAttach();
		virtual void onDetach();

		virtual void onUpdate(Ref<RenderContext>& context);

	private:
		std::vector<Ref<Entity>> m_ents;
	};

	template<>
	bool Asset::is<Scene>() const;
}

#endif