#ifndef __PIONNER_ASSET_ASSET_H__
#define __PIONNER_ASSET_ASSET_H__

#include "Base.h"

namespace pio
{
	enum class AssetType : uint8_t
	{
		None = 0, Camera, Mesh, Material, Scene
	};

	#define OVERRIDE_ASSET_TYPE(TypeName)  public:\
									       static  AssetType StaticType() { return TypeName; }\
									       virtual AssetType assetType() const override { return StaticType(); }

	class Asset
	{
	public:
		Asset() : m_id(), m_parentId(0) {}

		Asset(const Asset& rhs) : m_id(rhs.m_id), m_parentId(rhs.m_parentId) {}
		Asset operator=(const Asset& rhs)
		{
			if (this != &rhs)
			{
				m_id = rhs.m_id;
				m_parentId = rhs.m_parentId;
			}
			return *this;
		}

		virtual ~Asset() = default;
		virtual AssetType assetType() const { return StaticType(); }

	public:
		UUID32 id() { return m_id; }
		const UUID32& id() const { return m_id; }
		const UUID32& parentId() const { return m_parentId; }

		void setParentId(const UUID32& id) { m_parentId = id; }

	public:
		template<typename T>
		bool is() const { return false; }

		template<typename T>
		T* as() { if (std::is_base_of<Asset, T>() && is<T>()) { return static_cast<T*>(this); } else { return nullptr; } }

	public:
		static AssetType StaticType() { return AssetType::None; }

	protected:
		UUID32 m_id, m_parentId;
	};
}

#endif
