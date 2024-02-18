#include "Material.h"

#include "gfx/rhi/RenderAPI.h"
#include "platform/opengl/GLMaterial.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Material"

namespace pio
{
	Ref<Material> Material::Create(const std::string &name)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLMaterial>(name);
			default:
				return Ref<Material>();
		}
	}
}