#ifndef __PIONNER_SCENE_FACTORY_H__
#define __PIONNER_SCENE_FACTORY_H__

#include "Entry.h"

namespace pio
{
	class Scene;
	class Entity;
	struct TriangleMesh;

	class Factory
	{
	public:
		static Ref<Entity> MakeCamera(Ref<Scene>& scene, const std::string& name = "", int32_t depth = 0);
		static Ref<Entity> MakePlane(Ref<Scene>& scene, const std::string& name = "");

	private:
		Factory() {}
		~Factory() = default;
	};

	class Geometry3dFactory
	{
	public:
		static TriangleMesh MakePlane(uint32_t n = 5);

	private:
		Geometry3dFactory() {}
		~Geometry3dFactory() = default;
	};
}

#endif