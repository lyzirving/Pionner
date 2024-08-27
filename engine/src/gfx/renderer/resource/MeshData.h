#ifndef __PIONNER_GFX_RENDERER_RESOURCE_MESH_DATA_H__
#define __PIONNER_GFX_RENDERER_RESOURCE_MESH_DATA_H__

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

namespace pio
{
	class UniformData;
	class RenderContext;
	class Material;

	struct MeshData
	{
		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<IndexBuffer> Ebo;

		MeshData() {}

		MeshData(const MeshData& rhs);
		MeshData(MeshData&& rhs) noexcept;

		MeshData& operator=(const MeshData& rhs);
		MeshData& operator=(MeshData&& rhs) noexcept;

		bool valid() const { return Vao.use_count() != 0 && Vbo.use_count() != 0 && Ebo.use_count() != 0; }

		template<typename VertexType, typename IndiceType>
		static bool Create(Ref<RenderContext>& context, MeshData& buff, const std::vector<VertexType>& vertexList, const std::vector<IndiceType>& indice)
		{
			if (vertexList.empty() || indice.empty())
			{
			#ifdef LOCAL_TAG
			#undef LOCAL_TAG
			#endif
			#define LOCAL_TAG "MeshData"
				LOGW("warning! invalid vertex list[%s] or indice list[%s]", vertexList.empty() ? "true" : "false", indice.empty() ? "true" : "false");
			}
			buff.Vbo = VertexBuffer::Create(context, vertexList.data(), vertexList.size() * sizeof(VertexType), BufferUsage::Static);
			buff.Vbo->setBufferLayout(VertexBuffer::To<VertexType>());
			buff.Ebo = IndexBuffer::Create(context, indice.data(), indice.size() * sizeof(IndiceType), indice.size(), BufferUsage::Static);
			buff.Vao = VertexArray::Create(context);
			buff.Vao->addVertexBuffer(buff.Vbo);
			return true;
		}
	};

	struct MeshCmd
	{
		MeshData Data;
		Ref<Material> Material;
		std::map<std::string, Ref<UniformData>> Uniforms;
	};
}

#endif