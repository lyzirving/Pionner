#ifndef __PIONNER_SCENE_FACTORY_H__
#define __PIONNER_SCENE_FACTORY_H__

#include "gfx/resource/TriangleMesh.h"

namespace pio
{
	class Factory
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
		Factory() {}
		~Factory() = default;
	};
}

#endif