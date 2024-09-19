#ifndef __PIONNER_ASSET_ASSET_H__
#define __PIONNER_ASSET_ASSET_H__

#include "Common.h"

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
		RenderTarget,
		Texture
	};

	enum class ImageType : uint8_t
	{
		PNG,
		JPG
	};

	#define PIO_DEFINE_ASSET_TYPE(TypeName)  public:\
									         static  AssetType StaticType() { return TypeName; }\
									         virtual AssetType assetType() const override { return StaticType(); }

	class Asset : public std::enable_shared_from_this<Asset>
	{
		PIO_DECLARE_IS_AS(Asset)
	public:
		Asset();
		virtual ~Asset() = default;

		Asset(const Asset& rhs);
		Asset(Asset&& rhs) noexcept;
		Asset operator=(const Asset& rhs);
		Asset operator=(Asset&& rhs) noexcept;
		
		virtual AssetType assetType() const { return AssetType::None; }
		virtual Ref<Asset> clone() const { return Ref<Asset>(); }

		template<typename T>
		Ref<T> self() { return RefCast<Asset, T>(shared_from_this()); }

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
