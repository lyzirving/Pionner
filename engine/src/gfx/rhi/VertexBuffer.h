#ifndef __PIONNER_GFX_RHI_VERTEX_BUFFER_H__
#define __PIONNER_GFX_RHI_VERTEX_BUFFER_H__

#include "RhiDef.h"
#include "gfx/struct/Mesh.h"
#include "gfx/struct/Geometry2D.h"

namespace pio
{
	struct VertexBufferElement
	{
		ShaderDataType Type{ ShaderDataType::None };
		uint32_t Component{ 0 };
		std::string Name{};
		uint32_t Size{ 0 };
		size_t Offset{ 0 };
		bool Normalized{ false };

		VertexBufferElement() = default;

		VertexBufferElement(const std::string &name, ShaderDataType type, bool normalized)
			: Name(name), Type(type), Component(RhiDef::GetShaderDataTypeCompNum(type))
			, Size(RhiDef::GetShaderDataTypeByteSize(type))
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

		inline std::vector<VertexBufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<VertexBufferElement>::iterator end() { return m_elements.end(); }
		inline std::vector<VertexBufferElement>::const_iterator begin() const { return m_elements.cbegin(); }
		inline std::vector<VertexBufferElement>::const_iterator end() const { return m_elements.cend(); }
		inline uint32_t getStride() const { return m_stride; }
		inline const std::vector<VertexBufferElement> &getElements() const { return m_elements; }
		inline uint32_t elementSize() const { return m_elements.size(); }
		inline bool valid() const { return m_stride != 0; }

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

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual bool init() = 0;
		virtual void destroy() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void setData(const void *data, uint32_t size, uint32_t offset = 0) = 0;
		virtual void setLayout(const VertexBufferLayout &layout) = 0;

		virtual bool isInit() const = 0;
		virtual const VertexBufferLayout &getLayout() const = 0;
		virtual uint32_t getSize() const = 0;

	public:
		template<typename T>
		static VertexBufferLayout To() { return VertexBufferLayout(); }

	public:
		static Ref<VertexBuffer> Create(uint32_t size, BufferUsage usage = BufferUsage::Static);
		static Ref<VertexBuffer> Create(const void *data, uint32_t size, BufferUsage usage = BufferUsage::Static);
	};

	template<>
	VertexBufferLayout VertexBuffer::To<Vertex>();

	template<>
	VertexBufferLayout VertexBuffer::To<BoneInfluence>();

	template<>
	VertexBufferLayout VertexBuffer::To<LineVertex>();

	template<>
	VertexBufferLayout VertexBuffer::To<QuadVertex>();
}

#endif