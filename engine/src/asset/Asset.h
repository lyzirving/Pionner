#ifndef __PIONNER_ASSET_ASSET_H__
#define __PIONNER_ASSET_ASSET_H__

#include "Entry.h"

namespace pio
{
	enum class AssetType : uint8_t
	{
		None, 
		// Asset that has Cpu resource
		SceneResource, 
		Camera,
		Mesh, 
		Material,
		// Asset that has Gpu resource
		RenderResource, 
		MeshRenderBuffer, 
		Texture
	};

	#define OVERRIDE_ASSET_TYPE(TypeName)  public:\
									       static  AssetType StaticType() { return TypeName; }\
									       virtual AssetType assetType() const override { return StaticType(); }

	class Asset
	{
		PIO_IS_AS_INTERFACE_DECLARE(Asset)
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
		virtual AssetType assetType() const { return AssetType::None; }

	public:
		UUID32 assetHnd() { return m_hnd; }
		const UUID32& assetHnd() const { return m_hnd; }
		const UUID32& parentHnd() const { return m_parentHnd; }

		void setParentHnd(const UUID32& hnd) { m_parentHnd = hnd; }		

	protected:
		UUID32 m_hnd, m_parentHnd;
	};
}

#endif
