#include "VertexBuffer.h"

#include "gfx/rhi/RenderAPI.h"

#include "platform/opengl/GLVertexBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VertexBuffer"

namespace pio
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, BufferUsage usage)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<VertexBuffer, GLVertexBuffer>(size, usage);
			default:
				return Ref<VertexBuffer>();
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void *data, uint32_t size, BufferUsage usage)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<VertexBuffer, GLVertexBuffer>(data, size, usage);
			default:
				return Ref<VertexBuffer>();
		}
	}

	template<>
	VertexBufferLayout VertexBuffer::To<Vertex>()
	{
		VertexBufferElement pos{ "Position", ShaderDataType::Float3, false };
		VertexBufferElement normal{ "Normal", ShaderDataType::Float3, false };
		VertexBufferElement tagent{ "Tangent", ShaderDataType::Float3, false };
		VertexBufferElement binormal{ "Binormal", ShaderDataType::Float3, false };
		VertexBufferElement texCoord{ "Texcoord", ShaderDataType::Float2, false };
		VertexBufferLayout layout{ pos, normal, tagent, binormal, texCoord };
		return layout;
	}

	template<>
	VertexBufferLayout VertexBuffer::To<BoneInfluence>()
	{
		VertexBufferElement weights{ "Weights", ShaderDataType::Float4, false };
		VertexBufferElement boneInfoIndices{ "BoneInfoIndices", ShaderDataType::UInt4, false };
		VertexBufferLayout layout{ weights, boneInfoIndices };
		return layout;
	}

	template<>
	VertexBufferLayout VertexBuffer::To<LineVertex>()
	{
		VertexBufferElement pos{ "Position", ShaderDataType::Float3, false };
		VertexBufferElement color{ "Color", ShaderDataType::Float4, false };
		VertexBufferLayout layout{ pos, color };
		return layout;
	}

	template<>
	VertexBufferLayout VertexBuffer::To<QuadVertex>()
	{
		VertexBufferElement pos{ "Position", ShaderDataType::Float3, false };
		VertexBufferElement texcoord{ "Texcoord", ShaderDataType::Float2, false };
		VertexBufferLayout layout{ pos, texcoord };
		return layout;
	}

	template<>
	VertexBufferLayout VertexBuffer::To<ViewVertex>()
	{
		VertexBufferElement pos{ "Position", ShaderDataType::Float3, false };
		VertexBufferElement texcoord{ "Texcoord", ShaderDataType::Float2, false };
		VertexBufferLayout layout{ pos, texcoord };
		return layout;
	}
}