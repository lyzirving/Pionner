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

	class GeometryFactory
	{
	public:
		/*
		* @brief	Make a plane whose length is n and laies on xz plane, the shape of the plane is square
		* @param n	length of the square
		*/
		static TriangleMesh MakePlane(uint32_t n = 5);
		static TriangleMesh MakeScreenQuad();


	private:
		GeometryFactory() {}
		~GeometryFactory() = default;
	};
}

#endif