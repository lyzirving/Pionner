#ifndef __PIONNER_GFX_STRUCT_GEOMETRY_H__
#define __PIONNER_GFX_STRUCT_GEOMETRY_H__

#include "Mesh.h"

namespace pio
{
	class Geometry : public MeshSource
	{
		OVERRIDE_ASSET_TYPE(AssetType::Geometry)
	public:
		Geometry(const std::string &name) : MeshSource(name) {}
		virtual ~Geometry() = default;

		void setColor(const glm::vec3 &color);
		void setAlpha (float alpha);
		Ref<Material> getMaterial() const;

		void invalidate(bool val = true) { m_invalidate = val; }
		virtual void flush();

	protected:
		virtual void build() {}

		bool needUpdate() { return m_invalidate; }

	protected:
		bool m_invalidate{ true };

	private:
		friend class MeshFactory;
	};

	class Cylinder : public Geometry
	{
		OVERRIDE_ASSET_TYPE(AssetType::Cylinder)

	public:
		Cylinder(const std::string &name) : Geometry(name) {}
		virtual ~Cylinder() = default;

	public:
		inline float getHeight() const { return m_height; }
		inline float getRadius() const { return m_radius; }

	private:
		float m_radius{ 0.05f }, m_height{ 0.5f };

	private:
		friend class MeshFactory;
	};

	class Cone : public Geometry
	{
		OVERRIDE_ASSET_TYPE(AssetType::Cone)

	public:
		Cone(const std::string &name) : Geometry(name) {}
		virtual ~Cone() = default;

	public:
		inline float getHeight() const { return m_height; }
		inline float getRadius() const { return m_radius; }

	private:
		float m_radius{ 0.05f }, m_height{ 0.5f };

	private:
		friend class MeshFactory;
	};

	class Arrow3D : public Geometry
	{
		OVERRIDE_ASSET_TYPE(AssetType::Arrow)

	public:
		Arrow3D(const std::string &name) : Geometry(name) {}
		virtual ~Arrow3D() = default;

	private:
		float m_cylinRadius{ 0.05f }, m_cylinHeight{ 0.5f };
		float m_coneRadius{ 0.05f }, m_coneHeight{ 0.5f };

	private:
		friend class MeshFactory;
	};

	class Sphere : public Geometry
	{
		OVERRIDE_ASSET_TYPE(AssetType::Sphere)

	public:
		Sphere(const std::string &name) : Geometry(name) {}
		virtual ~Sphere() = default;

		void setRadius(float radius);

	protected:
		virtual void build() override;

	private:
		float m_radius{ 0.f };
		uint32_t m_horItrCnt{ 0 }, m_verItrCnt{ 0 };

	private:
		friend class MeshFactory;
	};

	/* Triangle segments organized as a circle on XY plane in counter-clockwise order */
	class Torus : public Geometry
	{
		OVERRIDE_ASSET_TYPE(AssetType::Torus)

	public:
		Torus(const std::string &name) : Geometry(name) {}
		virtual ~Torus() = default;

		void setRadius(float radius);

	protected:
		virtual void build() override;

		static void MakeRingVertex(const glm::vec2 &posXY, float radius, uint32_t itr, std::vector<Vertex> &out);

	private:
		float m_radius{ 0.f };
		float m_ringRadius{ 0.f };
		uint32_t m_itrCnt{ 0 };
		uint32_t m_ringItrCnt{ 0 };

	private:
		friend class MeshFactory;
	};

	template<>
	bool Asset::is<Geometry>() const;

	template<>
	bool Asset::is<Cylinder>() const;

	template<>
	bool Asset::is<Cone>() const;

	template<>
	bool Asset::is<Arrow3D>() const;

	template<>
	bool Asset::is<Sphere>() const;

	template<>
	bool Asset::is<Torus>() const;
}

#endif