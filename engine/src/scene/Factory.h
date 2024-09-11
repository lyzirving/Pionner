#ifndef __PIONNER_SCENE_FACTORY_H__
#define __PIONNER_SCENE_FACTORY_H__

#include "Common.h"
#include "gfx/resource/TriangleMesh.h"

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
		static TriangleMesh3d MakePlane(float n = 5.f);
		static TriangleMesh3d MakeCube(float n = 2.f);
		static TriangleMesh3d MakeScreenQuad();

	private:
		static void MakeTriangleMesh3d(const Vertex3d& v0, const Vertex3d& v1, const Vertex3d& v2, const Vertex3d& v3, TriangleMesh3d& triangles);

	private:
		GeometryFactory() {}
		~GeometryFactory() = default;
	};
}

#endif