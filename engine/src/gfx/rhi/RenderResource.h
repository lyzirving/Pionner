#pragma once

#include "gfx/GfxDef.h"
#include "asset/Asset.h"

namespace pio
{
	class RenderContext;
	
	struct ResourceGCDelegate
	{
		RenderResourceType Type{ RenderResourceType::Num };
		uint32_t Id{ 0 };
		std::string Name{};

		ResourceGCDelegate() {}
		ResourceGCDelegate(RenderResourceType t, uint32_t id, const std::string& name) : Type(t), Id(id), Name(name) {}
	};

	class RenderResource : public Object
	{
		RTTR_ENABLE(Object)
	public:
		RenderResource(const Ref<RenderContext>& context, RenderResourceType type, const std::string& name = "");
		virtual ~RenderResource();

		virtual bool Init() = 0;
		virtual void Destroy() = 0;
		virtual bool IsInit() const = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual bool IsBind() const { return m_Bound; }

		virtual uint32_t Size() const { return 0; }
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) {}
		virtual void SetIndice(const void* data, uint32_t size, uint32_t indiceNum, uint32_t offset = 0) {}

		virtual uint32_t Id() const { return m_ID; }

		WeakRef<RenderContext>& Context() { return m_Context; }
		RenderResourceType Type() const { return m_Type; }		
		const std::string& Name() const { return m_Name; }

	private:
		static std::string CreateResourceName(RenderResourceType type);
		static void AddCounter(RenderResourceType type);
		static uint32_t k_ResourceCounter[PIO_UINT8(RenderResourceType::Num)];

	protected:
		WeakRef<RenderContext> m_Context;
		std::string m_Name{};
		RenderResourceType m_Type;
		uint32_t m_ID{ 0 }, m_Size{ 0 };
		bool m_Bound{ false };
		void* m_Data{ nullptr };
	};
}