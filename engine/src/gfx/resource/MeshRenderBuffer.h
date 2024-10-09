#ifndef __PIONNER_GFX_RESOURCE_MESH_RENDER_BUFFER_H__
#define __PIONNER_GFX_RESOURCE_MESH_RENDER_BUFFER_H__

#include "gfx/GfxDef.h"
#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"
#include "gfx/rhi/UniformData.h"

namespace pio
{	
	class Mesh;
	class Shader;
	class RenderContext;

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
		void bind(Ref<Shader>& shader);	

		void onUpdate(Ref<RenderContext>& context);

		template<typename VertexType, typename IndiceType>
		void setUp(Ref<RenderContext>& context, const std::vector<VertexType>& vertice, const std::vector<IndiceType>& indice)
		{
			if (valid())
				return;

			MeshRenderBuffer::Create(context, *this, vertice, indice);

			auto transUnim = CreateRef<UniformMat4>(GpuAttr::UNI_MODEL_MAT);
			Uniforms.insert({ transUnim->name(), transUnim });
		}

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
			buff.Vbo = VertexBuffer::Create(context, &vertexList[0], vertexList.size() * sizeof(VertexType), BufferUsage::Dynamic);
			buff.Vbo->setBufferLayout(VertexBuffer::To<VertexType>());
			buff.Ebo = IndexBuffer::Create(context, &indice[0], indice.size() * sizeof(IndiceType), indice.size(), Rhi::GetIndexInternalFmt<IndiceType>(), BufferUsage::Dynamic);
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