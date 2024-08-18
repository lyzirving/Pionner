#ifndef __PIONNER_SCENE_FACTORY_H__
#define __PIONNER_SCENE_FACTORY_H__

#include "Base.h"

namespace pio
{
	class Scene;
	class Entity;

	class Factory
	{
	public:
		static Ref<Entity> MakeCamera(Ref<Scene>& scene, const std::string& name = "", bool bMain = false);

	private:
		Factory() {}
		~Factory() = default;
	};
}

#endif