#ifndef __PIONNER_SCENE_FACTORY_H__
#define __PIONNER_SCENE_FACTORY_H__

#include "gfx/resource/MeshData.h"

namespace pio
{
	class Factory
	{
	public:
		/*
		* @brief	Make a plane that laies on xz plane, the shape of the plane is rectangle
		* @param w	width of the plane, stands for column
		* @param h  height of the plane, stands for row
		*/
		static Ref<TriangleMesh> MakePlane(float w = 10.f, float h = 7.f);
		static Ref<TriangleMesh> MakeCube(float n = 2.f);
		static Ref<TriangleMesh> MakeSphere(float radius, int32_t itr = 16);
		/*
		* @brief	Make a square in XY plane and plane's center is (0, 0).
		* @param w	width of plane
		* @param h	height of plane
		*/
		static Ref<TriangleMesh> MakeSquare(float w, float h);		
		static Ref<TriangleMesh> MakeScreenQuad();

		static Ref<LineSeqMesh> MakeDirLightGizmo(float r, float len);

	private:
		static void MakeTriangleMesh(const Vertex3d& v0, const Vertex3d& v1, const Vertex3d& v2, const Vertex3d& v3, Ref<TriangleMesh>& triangles);

	private:
		Factory() {}
		~Factory() = default;
	};
}

#endif