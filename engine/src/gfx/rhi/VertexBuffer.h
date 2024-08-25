#ifndef __PIONNER_GFX_RHI_VERTEX_BUFFER_H__
#define __PIONNER_GFX_RHI_VERTEX_BUFFER_H__

#include "RenderResource.h"

namespace pio
{
	struct VertexBufferElement
	{
		ShaderDataType Type{ ShaderDataType::None };
		uint32_t Comp{ 0 };
		std::string Name{};
		uint32_t Size{ 0 };
		size_t Offset{ 0 };
		bool Normalized{ false };

		VertexBufferElement() = default;

		VertexBufferElement(const std::string &name, ShaderDataType type, bool normalized)
			: Name(name), Type(type), Comp(Rhi::GetShaderDataTypeCompNum(type))
			, Size(Rhi::GetShaderDataTypeByteSize(type))
			, Offset(0), Normalized(normalized)
		{
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() {}
		VertexBufferLayout(const std::vector<VertexBufferElement> &elems) : m_elements(elems)
		{ 
			calculateOffsetsAndStride(); 
		}
		VertexBufferLayout(std::initializer_list<VertexBufferElement> elems) : m_elements(elems)
		{
			calculateOffsetsAndStride();
		}
		~VertexBufferLayout() = default;

		std::vector<VertexBufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return m_elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return m_elements.cbegin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return m_elements.cend(); }

		uint32_t stride() const { return m_stride; }
		bool valid() const { return m_stride != 0; }
		const std::vector<VertexBufferElement> &elements() const { return m_elements; }
		uint32_t elementSize() const { return m_elements.size(); }		

	private:
		void calculateOffsetsAndStride()
		{
			m_stride = 0;
			for (auto &element : m_elements)
			{
				element.Offset = m_stride;
				m_stride += element.Size;
			}
		}

	private:
		std::vector<VertexBufferElement> m_elements{};
		uint32_t m_stride{ 0 };
	};

	class VertexBuffer : public RenderResource
	{
	public:
		VertexBuffer(Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::VBO) {}
		virtual ~VertexBuffer() = default;

		virtual void setLayout(const VertexBufferLayout &layout) = 0;
		virtual const VertexBufferLayout &layout() const = 0;

	public:
		template<typename T>
		static VertexBufferLayout To() { return VertexBufferLayout(); }

	public:
		static Ref<VertexBuffer> Create(Ref<RenderContext>& context, uint32_t size, BufferUsage usage = BufferUsage::Static);
		static Ref<VertexBuffer> Create(Ref<RenderContext>& context, const void* data, uint32_t size, BufferUsage usage = BufferUsage::Static);
	};

	template<>
	VertexBufferLayout VertexBuffer::To<Vertex3d>();
}

#endif