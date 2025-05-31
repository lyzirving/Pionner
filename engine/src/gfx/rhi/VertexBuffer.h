#pragma once

#include "RenderResource.h"

namespace pio
{
	struct VertexBufferElement
	{
		ShaderDataType Type{ ShaderDataType::Byte };
		uint32_t Comp{ 0 };
		std::string Name{};
		uint32_t Size{ 0 };
		size_t Offset{ 0 };
		bool Normalized{ false };

		VertexBufferElement() = default;

		VertexBufferElement(const std::string &name, ShaderDataType type, bool normalized)
			: Name(name), Type(type), Comp(GetShaderDataTypeCompNum(type))
			, Size(GetShaderDataTypeByteSize(type))
			, Offset(0), Normalized(normalized)
		{
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() {}
		VertexBufferLayout(const std::vector<VertexBufferElement> &elems) : m_Elements(elems)
		{ 
			CalculateOffsetsAndStride(); 
		}
		VertexBufferLayout(std::initializer_list<VertexBufferElement> elems) : m_Elements(elems)
		{
			CalculateOffsetsAndStride();
		}
		~VertexBufferLayout() = default;

		std::vector<VertexBufferElement>::iterator Begin() { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::iterator End() { return m_Elements.end(); }
		std::vector<VertexBufferElement>::const_iterator Begin() const { return m_Elements.cbegin(); }
		std::vector<VertexBufferElement>::const_iterator End() const { return m_Elements.cend(); }

		uint32_t Stride() const { return m_Stride; }
		bool Valid() const { return m_Stride != 0; }
		const std::vector<VertexBufferElement> &Elements() const { return m_Elements; }
		uint32_t ElementSize() const { return m_Elements.size(); }		

	private:
		void CalculateOffsetsAndStride()
		{
			m_Stride = 0;
			for (auto &element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<VertexBufferElement> m_Elements{};
		uint32_t m_Stride{ 0 };
	};

	class VertexBuffer : public RenderResource
	{
		RTTR_ENABLE(RenderResource)
	public:
		VertexBuffer(const Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::VBO) {}
		virtual ~VertexBuffer() = default;

		virtual void SetBufferLayout(const VertexBufferLayout &layout) = 0;
		virtual const VertexBufferLayout &Layout() const = 0;

	public:
		template<typename T>
		static VertexBufferLayout To() { return VertexBufferLayout(); }

	public:
		static Ref<VertexBuffer> Create(const Ref<RenderContext>& context, uint32_t size, BufferUsage usage = BufferUsage::Static);
		static Ref<VertexBuffer> Create(const Ref<RenderContext>& context, const void* data, uint32_t size, BufferUsage usage = BufferUsage::Static);
	};

	template<>
	VertexBufferLayout VertexBuffer::To<Vertex>();

	template<>
	VertexBufferLayout VertexBuffer::To<BoneInfluence>();
}