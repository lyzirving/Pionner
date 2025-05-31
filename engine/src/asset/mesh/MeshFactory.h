#pragma once

#include "asset/def/AssetDef.h"

namespace pio
{
	class MeshFactory
	{
	public:
		/*
		* @brief	Make a plane that laies on xz plane, the shape of the plane is rectangle
		* @param w	width of the plane, stands for column
		* @param h  height of the plane, stands for row
		*/
		static SubMesh MakePlane(float w = 1.f, float h = 1.f);
		static SubMesh MakeCube(float n = 1.f);
		static SubMesh MakeSphere(float radius = 1.f, int32_t itr = 32);
		static SubMesh MakeScreenQuad();
		static SubMesh MakeDirLightGizmo(float r, float len);
		static SubMesh MakePointLightGizmo(float r, int32_t itr = 32);

	private:
		static void MakeVertexMesh(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3, SubMesh& subMesh);
		static void CalcTangent(Vertex& v0, Vertex& v1, Vertex& v2);

	private:
		MeshFactory() {}
		~MeshFactory() = default;
	};
}