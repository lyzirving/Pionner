#include "RenderPass.h"

#include "RenderAPI.h"
#include "UniformBufferSet.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/struct/MeshBuilder.h"

#include "platform/opengl/GLRenderPass.h"
#include "asset/AssetsManager.h"

#include "gfx/core/Skybox.h"
#include "scene/Light.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPass"

namespace pio
{
	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification &spec)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLRenderPass>(spec);
			default:
				return Ref<RenderPass>();
		}
	}
}