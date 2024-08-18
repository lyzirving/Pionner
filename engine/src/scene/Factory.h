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
		static Ref<Entity> MakeCamera(Ref<Scene>& scene, const std::string& name = "", int32_t depth = 0);
		static Ref<Entity> MakePlane(Ref<Scene>& scene, const std::string& name = "");

	private:
		Factory() {}
		~Factory() = default;
	};
}

#endif