#pragma once

#include "def/AssetDef.h"

namespace pio
{
	class Asset : public Object
	{
		RTTR_ENABLE(Object)
	public:
		Asset();
		Asset(const ImportParams& params);
		virtual ~Asset() = default;

		Asset(const Asset& rhs);
		Asset(Asset&& rhs) noexcept;
		Asset operator=(const Asset& rhs);
		Asset operator=(Asset&& rhs) noexcept;
		
		//only used for initialization
		virtual void Init();
		virtual bool IsInit() const { return false; }				
		virtual void OnAttrsChange() {}
		virtual Ref<Asset> SpawnChild() { return Ref<Asset>(); }

	public:
		UUID32 AssetHnd() { return m_Hnd; }
		const UUID32& AssetHnd() const { return m_Hnd; }
		const UUID32& ParentHnd() const { return m_ParentHnd; }
		const std::string& FullPath() const { return m_FullPath; }
		const std::string& Key() const { return m_Key; }
		const std::string& Name() const { return m_Params.Name; }
		const std::string& Suffix() const { return m_Params.Suffix; }
		
		void SetParentHnd(const UUID32& hnd) { m_ParentHnd = hnd; }

	protected:
		virtual void OnInit() {}

	protected:
		UUID32 m_Hnd, m_ParentHnd;
		ImportParams m_Params;
		std::string m_FullPath;
		std::string m_Key;
		WeakRef<RenderContext> m_Context;
		WeakRef<Asset> m_Owner;
	};
}
