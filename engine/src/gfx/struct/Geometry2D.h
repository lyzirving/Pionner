#ifndef __PIONNER_GFX_STRUCT_GEOMETRY2D_H__
#define __PIONNER_GFX_STRUCT_GEOMETRY2D_H__

#include "scene/SceneDef.h"

namespace pio
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	constexpr static const uint32_t LINE_MESH_LIMIT = 65535u;

	class Geometry2D : public Asset
	{
	public:
		Geometry2D(const std::string &name) : Asset(name) {}
		virtual ~Geometry2D() = default;

		virtual void clear() { Size = 0; };

	public:
		uint32_t Size{ 0 };
		uint32_t Capacity{ 0 };

		Ref<VertexArray>  VertexArray{};
		Ref<VertexBuffer> VertexBuffer{};
		Ref<IndexBuffer>  IndexBuffer{};
	};

	class LineMesh : public Geometry2D
	{
		OVERRIDE_ASSET_TYPE(AssetType::LineMesh)
	public:
		LineMesh(const std::string &name) : Geometry2D(name) {}
		virtual ~LineMesh() = default;

		virtual void clear() override;

	public:
		std::vector<LineVertex> Vertex{};
		std::vector<uint32_t> Indices{};
	};

	class QuadMesh : public Geometry2D
	{
		OVERRIDE_ASSET_TYPE(AssetType::QuadMesh)
	public:
		QuadMesh(const std::string &name) : Geometry2D(name) {}
		virtual ~QuadMesh() = default;

		virtual void clear() override;

	public:
		std::vector<QuadVertex> Vertex{};
		std::vector<uint32_t> Indices{};
	};

	template<>
	bool Asset::is<LineMesh>() const;

	template<>
	bool Asset::is<QuadMesh>() const;
}

#endif