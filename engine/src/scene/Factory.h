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
		/*
		* @brief	Make a square in XY plane and plane's center is (0, 0).
		* @param w	width of plane
		* @param h	height of plane
		*/
		static TriangleMesh3d MakeSquare(float w, float h);
		static TriangleMesh3d MakeScreenQuad();

	private:
		static void MakeTriangleMesh3d(const Vertex3d& v0, const Vertex3d& v1, const Vertex3d& v2, const Vertex3d& v3, TriangleMesh3d& triangles);

	private:
		Factory() {}
		~Factory() = default;
	};
}

#endif