#include "Shader.h"

#include "gfx/rhi/RenderAPI.h"

#include "platform/opengl/GLShader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

namespace pio
{
	Ref<Shader> Shader::Create(const char *name, const char *vert, const char *frag)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<Shader, GLShader>(name, vert, frag);
			default:
				return Ref<Shader>();
		}
	}

	Ref<Shader> Shader::Create(const char *name, const char *vert, const char *frag, const char *geo)
	{
		switch (RenderAPI::CurrentType()) 
		{
			case RenderAPI::Type::OpenGL: 
				return CreateRef<Shader, GLShader>(name, vert, frag, geo);
			default:
				return Ref<Shader>(); 
		}
	}
}