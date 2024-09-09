#ifndef __PIONNER_SCENE_FACTORY_H__
#define __PIONNER_SCENE_FACTORY_H__

#include "Entry.h"
#include "scene/resources/TriangleMesh.h"

namespace pio
{
	class Scene;
	class Entity;
	class RenderContext;

	class Factory
	{
	public:
		static Ref<Entity> MakeCamera(Ref<RenderContext>& context, Ref<Scene>& scene, const std::string& name = "", int32_t depth = 0);
		static Ref<Entity> MakePlane(Ref<RenderContext>& context, Ref<Scene>& scene, const std::string& name = "");

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
		static TriangleMesh3d MakePlane(uint32_t n = 5);
		static TriangleMesh3d MakeScreenQuad();

	private:
		GeometryFactory() {}
		~GeometryFactory() = default;
	};
}

#endif