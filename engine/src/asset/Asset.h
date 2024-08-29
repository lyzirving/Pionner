#ifndef __PIONNER_ASSET_ASSET_H__
#define __PIONNER_ASSET_ASSET_H__

#include "Entry.h"

namespace pio
{
	enum class AssetType : uint8_t
	{
		None, 
		SceneResource, Camera, Mesh, Material,
		RenderResource, Texture
	};

	#define OVERRIDE_ASSET_TYPE(TypeName)  public:\
									       static  AssetType StaticType() { return TypeName; }\
									       virtual AssetType assetType() const override { return StaticType(); }

	class Asset
	{
	public:
		Asset() : m_hnd(), m_parentHnd(0) {}

		Asset(const Asset& rhs) : m_hnd(rhs.m_hnd), m_parentHnd(rhs.m_parentHnd) {}
		Asset operator=(const Asset& rhs)
		{
			if (this != &rhs)
			{
				m_hnd = rhs.m_hnd;
				m_parentHnd = rhs.m_parentHnd;
			}
			return *this;
		}

		virtual ~Asset() = default;
		virtual AssetType assetType() const { return StaticType(); }

	public:
		UUID32 assetHnd() { return m_hnd; }
		const UUID32& assetHnd() const { return m_hnd; }
		const UUID32& parentHnd() const { return m_parentHnd; }

		void setParentHnd(const UUID32& hnd) { m_parentHnd = hnd; }

	public:
		template<typename T>
		bool is() const { return false; }

		template<typename T>
		T* as() { if (std::is_base_of<Asset, T>() && is<T>()) { return static_cast<T*>(this); } else { return nullptr; } }

	public:
		static AssetType StaticType() { return AssetType::None; }

	protected:
		UUID32 m_hnd, m_parentHnd;
	};
}

#endif
