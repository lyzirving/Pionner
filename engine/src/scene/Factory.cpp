#include "Factory.h"

#include "Components.h"
#include "Scene.h"

#include "asset/AssetMgr.h"

#include "scene/3d/Camera.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Factory"

namespace pio
{
	Ref<Entity> Factory::MakeCamera(Ref<Scene>& scene, const std::string& name, int32_t depth)
	{
		auto entity	= scene->addEntity<CameraComponent>(name);
		auto *pComp = entity->getComponent<CameraComponent>();
		auto camera = AssetMgr::MakeRuntimeAsset<Camera>();

		camera->setDepth(depth);
		pComp->Handle = camera->id();
		return entity;
	}

	Ref<Entity> Factory::MakePlane(Ref<Scene>& scene, const std::string& name)
	{
		auto entity = scene->addEntity<MeshFilter, MeshRenderer>(name);
		return entity;
	}
}