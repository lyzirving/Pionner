#include "RenderResource.h"

#include "gfx/renderer/RenderContext.h"

namespace pio
{
	uint32_t RenderResource::k_ResourceCounter[PIO_UINT8(RenderResourceType::Num)] = { 0 };

	RenderResource::RenderResource(const Ref<RenderContext> &context, RenderResourceType type, const std::string& name) : Object(),
		m_Context(context), m_Type(type), m_Name(name)
	{
		if (m_Name.empty())
		{
			m_Name = CreateResourceName(m_Type);
		}
		AddCounter(m_Type);
	}

	RenderResource::~RenderResource()
	{
		auto ctx = m_Context.lock();
		if(ctx)
		{
			ResourceGCDelegate resource(m_Type, Id(), m_Name);
			ctx->RecyleResource(std::move(resource));
		}
	}

	std::string RenderResource::CreateResourceName(RenderResourceType type)
	{
		switch (type)
		{
			case RenderResourceType::UBO:
				return std::string("UBO-") + std::to_string(k_ResourceCounter[PIO_UINT8(type)]);
			case RenderResourceType::VBO:
				return std::string("VBO-") + std::to_string(k_ResourceCounter[PIO_UINT8(type)]);
			case RenderResourceType::EBO:
				return std::string("EBO-") + std::to_string(k_ResourceCounter[PIO_UINT8(type)]);
			case RenderResourceType::VAO:
				return std::string("VAO-") + std::to_string(k_ResourceCounter[PIO_UINT8(type)]);
			case RenderResourceType::FBO:
				return std::string("FBO-") + std::to_string(k_ResourceCounter[PIO_UINT8(type)]);
			case RenderResourceType::Texture:
				return std::string("Texture-") + std::to_string(k_ResourceCounter[PIO_UINT8(type)]);
			case RenderResourceType::RenderBuffer:
				return std::string("RenderBuffer-") + std::to_string(k_ResourceCounter[PIO_UINT8(type)]);
			case RenderResourceType::Shader:
				return std::string("Shader-") + std::to_string(k_ResourceCounter[PIO_UINT8(type)]);
			default:
				LOGE("err! undefined render resource type[%u]", type);
				std::abort();
				return std::string("Undefined");
		}
	}

	void RenderResource::AddCounter(RenderResourceType type)
	{
		k_ResourceCounter[PIO_UINT8(type)]++;
	}
}