#ifndef __PIONNER_GFX_RESOURCE_MESH_RENDER_BUFFER_H__
#define __PIONNER_GFX_RESOURCE_MESH_RENDER_BUFFER_H__

#include "gfx/GfxDef.h"
#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

namespace pio
{
	class RenderContext;
	class UniformData;
	class Mesh;
	class MeshRenderBuffer;

	class MeshRenderBuffer : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::MeshRenderBuffer)
	public:
		MeshRenderBuffer();

		MeshRenderBuffer(const MeshRenderBuffer& rhs);
		MeshRenderBuffer(MeshRenderBuffer&& rhs) noexcept;

		MeshRenderBuffer& operator=(const MeshRenderBuffer& rhs);
		MeshRenderBuffer& operator=(MeshRenderBuffer&& rhs) noexcept;

		bool valid() const { return Vao.use_count() != 0 && Vbo.use_count() != 0 && Ebo.use_count() != 0; }

		void setUp(Ref<RenderContext>& context, Ref<Mesh>& mesh);
		void update(Ref<RenderContext>& context);

	public:
		template<typename VertexType, typename IndiceType>
		static bool Create(Ref<RenderContext>& context, MeshRenderBuffer& buff, const std::vector<VertexType>& vertexList, const std::vector<IndiceType>& indice)
		{
			if (vertexList.empty() || indice.empty())
			{
			#ifdef LOCAL_TAG
			#undef LOCAL_TAG
			#endif
			#define LOCAL_TAG "MeshRenderBuffer"
				LOGW("warning! invalid vertex list[%s] or indice list[%s]", vertexList.empty() ? "true" : "false", indice.empty() ? "true" : "false");
			}
			buff.Vbo = VertexBuffer::Create(context, &vertexList[0], vertexList.size() * sizeof(VertexType), BufferUsage::Static);
			buff.Vbo->setBufferLayout(VertexBuffer::To<VertexType>());
			buff.Ebo = IndexBuffer::Create(context, &indice[0], indice.size() * sizeof(IndiceType), indice.size(), Rhi::GetIndexInternalFmt<IndiceType>(), BufferUsage::Static);
			buff.Vao = VertexArray::Create(context);
			buff.Vao->addVertexBuffer(buff.Vbo);
			return true;	
		}		

	public:
		Transform3D Transform;
		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<IndexBuffer> Ebo;
		std::map<std::string, Ref<UniformData>> Uniforms;
	};

	template<>
	bool Asset::is<MeshRenderBuffer>() const;
}

#endif