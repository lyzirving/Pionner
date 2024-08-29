#ifndef __PIONNER_GFX_RHI_RENDER_RESOURCE_H__
#define __PIONNER_GFX_RHI_RENDER_RESOURCE_H__

#include "RhiDef.h"
#include "asset/Asset.h"

namespace pio
{
	class RenderContext;
	
	struct ResourceGCDelegate
	{
		RenderResourceType Type{ RenderResourceType::Num };
		uint32_t Id{ 0 };

		ResourceGCDelegate() {}
		ResourceGCDelegate(RenderResourceType t, uint32_t id) : Type(t), Id(id) {}
	};

	class RenderResource : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::RenderResource)
	public:
		RenderResource(Ref<RenderContext>& context, RenderResourceType type);
		virtual ~RenderResource();

		virtual bool init() = 0;
		virtual void destroy() = 0;
		virtual bool isInit() const = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual uint32_t size() const = 0;
		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		RenderResourceType type() const { return m_type; }
		uint32_t id() const { return m_id; }

	protected:
		Ref<RenderContext> m_context;
		RenderResourceType m_type;
		uint32_t m_id{ 0 }, m_size{ 0 };
		void* m_data{ nullptr };
	};

	template<>
	bool Asset::is<RenderResource>() const;
}

#endif