#pragma once

#include "gfx/GfxDef.h"
#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

namespace pio
{	
	class Mesh;
	class Shader;
	class RenderContext;

	class MeshRenderBuffer : public Object
	{
		RTTR_ENABLE(Object)
	public:
		MeshRenderBuffer();

		MeshRenderBuffer(const MeshRenderBuffer& rhs);
		MeshRenderBuffer(MeshRenderBuffer&& rhs) noexcept;

		MeshRenderBuffer& operator=(const MeshRenderBuffer& rhs);
		MeshRenderBuffer& operator=(MeshRenderBuffer&& rhs) noexcept;

		bool Valid() const { return Vao.use_count() != 0 && Vbo.use_count() != 0 && Ebo.use_count() != 0; }

		template<typename VertexType, typename IndiceType>
		void Setup(const Ref<RenderContext>& context, const std::vector<VertexType>& vertice, const std::vector<IndiceType>& indice)
		{
			if (Valid())
				return;

			MeshRenderBuffer::Create(context, *this, vertice, indice);
		}

	public:
		template<typename VertexType, typename IndiceType>
		static bool Create(const Ref<RenderContext>& context, MeshRenderBuffer& buff, const std::vector<VertexType>& vertexList, const std::vector<IndiceType>& indice)
		{
			if (vertexList.empty() || indice.empty())
			{
			#ifdef LOCAL_TAG
			#undef LOCAL_TAG
			#endif
			#define LOCAL_TAG "MeshRenderBuffer"
				LOGW("warning! invalid vertex list[%s] or indice list[%s]", vertexList.empty() ? "true" : "false", indice.empty() ? "true" : "false");
			}
			buff.Vbo = VertexBuffer::Create(context, &vertexList[0], vertexList.size() * sizeof(VertexType), BufferUsage::Dynamic);
			buff.Ebo = IndexBuffer::Create(context, &indice[0], indice.size() * sizeof(IndiceType), indice.size(), GetIndexInternalFmt<IndiceType>(), BufferUsage::Dynamic);
			buff.Vao = VertexArray::Create(context);
			buff.Vbo->SetBufferLayout(VertexBuffer::To<VertexType>());
			buff.Vao->AddVertexBuffer(buff.Vbo);
			return true;	
		}		

	public:
		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<IndexBuffer> Ebo;
	};
}