#ifndef __PIONNER_ASSET_ASSET_H__
#define __PIONNER_ASSET_ASSET_H__

#include "Base.h"

namespace pio
{
	using AssetHandle = UUID32;

	enum class AssetType : uint8_t
	{
		None = 0, Camera, Scene
	};

	#define OVERRIDE_ASSET_TYPE(TypeName)  public:\
									       static  AssetType StaticType() { return TypeName; }\
									       virtual AssetType type() const override { return StaticType(); }

	class Asset
	{
	public:
		Asset() : m_handle(), m_parentHandle(0) {}

		Asset(const Asset& rhs) : m_handle(rhs.m_handle), m_parentHandle(rhs.m_parentHandle) {}
		Asset operator=(const Asset& rhs)
		{
			if (this != &rhs)
			{
				m_handle = rhs.m_handle;
				m_parentHandle = rhs.m_parentHandle;
			}
			return *this;
		}

		virtual ~Asset() = default;
		virtual AssetType type() const { return StaticType(); }

	public:
		AssetHandle handle() { return m_handle; }
		const AssetHandle& handle() const { return m_handle; }
		const AssetHandle& parentHandle() const { return m_parentHandle; }

		void setParentHandle(const AssetHandle& handle) { m_parentHandle = handle; }

	public:
		template<typename T>
		bool is() const { return false; }

		template<typename T>
		T* as() { if (std::is_base_of<Asset, T>() && is<T>()) { return static_cast<T*>(this); } else { return nullptr; } }

	public:
		static AssetType StaticType() { return AssetType::None; }

	protected:
		AssetHandle m_handle, m_parentHandle;
	};
}

#endif
