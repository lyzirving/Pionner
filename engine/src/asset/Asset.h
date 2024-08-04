#ifndef __PIONNER_ASSET_ASSET_H__
#define __PIONNER_ASSET_ASSET_H__

#include "core/Base.h"

namespace pio
{
	using AssetHandle = UUID32;

	static const char* ICON_MOVE_NORMAL = "move_normal";
	static const char* ICON_MOVE_SELECTED = "move_selected";
	static const char* ICON_ROTATE_NORMAL = "rotate_normal";
	static const char* ICON_ROTATE_SELECTED = "rotate_selected";

	enum class AssetType : uint8_t
	{
		None = 0,
		Mesh, StaticMesh, LineSegment, QuadMesh, CircleMesh, MeshSource,
		GeoBegin, Geometry, Cylinder, Cone, Arrow, Sphere, Torus, GeoEnd,
		Animation, Skeleton, Material,
		Texture, Texture2D, CubeTexture, CubeArrayTexture, BufferTexture, Skybox, RenderBuffer,
		PhysicsMat, PhysicsScene,
		Scene, Camera
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
		Asset(const std::string& name) : m_assetHandle(), m_parentHandle(0), m_name(name) {}

		Asset(const Asset& rhs) : m_assetHandle(rhs.m_assetHandle), m_parentHandle(rhs.m_parentHandle), m_name(rhs.m_name)
		{
		}

		Asset operator=(const Asset& rhs)
		{
			if (this != &rhs)
			{
				m_assetHandle = rhs.m_assetHandle;
				m_parentHandle = rhs.m_parentHandle;
				m_name = rhs.m_name;
			}
			return *this;
		}

		virtual ~Asset() = default;
		virtual AssetType getAssetType() const { return GetStaticType(); }

	public:
		AssetHandle getHandle() { return m_assetHandle; }
		const AssetHandle& getHandle() const { return m_assetHandle; }
		const AssetHandle& getParentHandle() const { return m_parentHandle; }
		const std::string& getName() const { return m_name; }
		void setParentHandle(const AssetHandle& handle) { m_parentHandle = handle; }
		void setName(const std::string& name) { m_name = name; }

	public:
		template<typename T>
		bool is() const { return false; }

		template<typename T>
		T* as() { if (std::is_base_of<Asset, T>() && is<T>()) { return static_cast<T*>(this); } else { return nullptr; } }

	public:
		static AssetType GetStaticType() { return AssetType::None; }

	protected:
		AssetHandle m_assetHandle;
		AssetHandle m_parentHandle;
		std::string m_name;
	};
}

#endif
