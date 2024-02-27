#ifndef __PIONNER_GFX_STRUCT_MESH_FACTORY_H__
#define __PIONNER_GFX_STRUCT_MESH_FACTORY_H__

#include "Mesh.h"
#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

namespace pio
{
	class MeshSource;
	class Geometry2D;

	class MeshFactory
	{
	public:
		static Ref<MeshSource> CreateCube(float len = 1.f);
		static Ref<MeshSource> CreatePlane(float width = 6.f, float height = 5.f);
		static Ref<MeshSource> CreateCylinder(float radius, float height, uint32_t itrCnt = 36);
		static Ref<MeshSource> CreateCone(float radius, float height, uint32_t itrCnt = 36);
		static Ref<MeshSource> CreateArrow3D(float cylinderRadius, float cylinderHeight, float coneRadius, float coneHeight, uint32_t itrCnt = 36);
		static Ref<MeshSource> CreateSphere(float radius, const glm::vec3 &color = glm::vec3(1.f, 0.f, 0.f), uint32_t horItrCnt = 36, uint32_t verItrCnt = 36);		
		static Ref<MeshSource> CreateTorus(float radius, float ringWidth, const glm::vec3 &color = glm::vec3(1.f, 0.f, 0.f), uint32_t itrCnt = 36, uint32_t ringItrCnt = 16);
		static Ref<Geometry2D> CreateFullScreenQuad();
		static Ref<Geometry2D> CreateScreenQuad(uint32_t l, uint32_t t, uint32_t r, uint32_t b, uint32_t screenWidth, uint32_t screenHeight);

	private:
		template<typename TypeV, typename TypeI>
		static void CreateBuffer(const Ref<MeshSource> &meshSource)
		{
			if (!meshSource)
				return;

			for (uint32_t i = 0; i < meshSource->m_submeshes.size(); i++)
			{
				auto &mesh = meshSource->m_submeshes[i];
				if (mesh.VertexCount > 0)
				{
					mesh.VertexBuffer = VertexBuffer::Create(&(meshSource->m_vertices[mesh.VertexOffset]),
															 mesh.VertexCount * sizeof(TypeV));
					mesh.VertexBuffer->setLayout(VertexBuffer::To<TypeV>());

					mesh.IndexBuffer = IndexBuffer::Create(&(meshSource->m_indices[mesh.IndexOffset]),
														   mesh.IndexCount * sizeof(TypeI),
														   mesh.IndexCount * 3);

					mesh.VertexArray = VertexArray::Create();
					mesh.VertexArray->addVertexBuffer(mesh.VertexBuffer);
				}
			}
		}

	private:
		MeshFactory() {}
		~MeshFactory() = default;
	};
}

#endif