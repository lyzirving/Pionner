#ifndef __PIONNER_ASSET_ASSET_H__
#define __PIONNER_ASSET_ASSET_H__

#include "core/Base.h"

namespace pio
{
	using AssetHandle = UUID32;

	enum class AssetType : uint8_t
	{
		None = 0, 
		Mesh, StaticMesh, LineMesh, QuadMesh, CircleMesh, MeshSource,
		GeoBegin, Geometry, Cylinder, Cone, Arrow, Sphere, Torus, GeoEnd,
		Animation, Skeleton, Material, 
		Texture, Texture2D, CubeTexture, CubeArrayTexture, BufferTexture, RenderBuffer,
		PhysicsMat, PhysicsScene
	};

	enum class AssetFmt : uint8_t
	{
		Obj = 0, Dae, GLTF, GLB, PNG, HDR, Num
	};

	#define OVERRIDE_ASSET_TYPE(type)  public:\
									   static  AssetType GetStaticType() { return type; }\
									   virtual AssetType getAssetType() const override { return GetStaticType(); }
                                       
	class Asset
	{
	public:
		Asset() : m_assetHandle(), m_parentHandle(0), m_name("None") {}
		Asset(const std::string &name) : m_assetHandle(), m_parentHandle(0), m_name(name) {}
		virtual ~Asset() = default;
		virtual AssetType getAssetType() const { return GetStaticType(); }

	public:
		inline AssetHandle getHandle() { return m_assetHandle; }
		inline const AssetHandle &getHandle() const { return m_assetHandle; }
		inline const AssetHandle &getParentHandle() const { return m_parentHandle; }
		inline const std::string &getName() const { return m_name; }
		inline void setParentHandle(const AssetHandle &handle) { m_parentHandle = handle; }
		inline void setName(const std::string &name) { m_name = name; }

	public:
		template<typename T>
		bool is() const { return false; }

		template<typename T>
		T *as() { if (std::is_base_of<Asset, T>() && is<T>()) { return static_cast<T *>(this); } else { return nullptr; } }

	public:
		static AssetType GetStaticType() { return AssetType::None; }

	protected:
		const AssetHandle m_assetHandle;
		AssetHandle m_parentHandle;
		std::string m_name;
	};
}

#endif
