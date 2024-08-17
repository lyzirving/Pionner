#include "Scene.h"
#include "Entity.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene"

namespace pio
{
	void Scene::onAttach()
	{
	}

	void Scene::onDetach()
	{
	}

	void Scene::onUpdate(Ref<RenderContext>& context)
	{
	}

	template<>
	bool Asset::is<Scene>() const { return type() == AssetType::Scene; }
}