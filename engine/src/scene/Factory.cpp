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
	Ref<Entity> Factory::MakeCamera(Ref<Scene>& scene, const std::string& name, bool bMain)
	{
		auto entity	= scene->addEntity<CameraComponent>(name);
		auto *pComp = entity->getComponent<CameraComponent>();
		pComp->Handle = AssetMgr::MakeRuntimeAsset<Camera>()->id();
		return entity;
	}
}