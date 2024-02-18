#include <sstream>

#include "UniformBuffer.h"

#include "core/math/MathLib.h"

#include "gfx/rhi/RenderAPI.h"
#include "gfx/rhi/Shader.h"

#include "platform/opengl/GLUniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformBuffer"

namespace pio
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding, BufferUsage usage)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLUniformBuffer>(size, binding, usage);
			default:
				return Ref<UniformBuffer>();
		}
	}

	bool UniformBuffer::Binding(const Ref<Shader> &shader, const std::string &blockName, uint32_t bindingPt)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return GLUniformBuffer::Binding(shader, blockName, bindingPt);
			default:
				return false;
		}
	}
}