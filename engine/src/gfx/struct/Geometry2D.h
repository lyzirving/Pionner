#ifndef __PIONNER_GFX_STRUCT_GEOMETRY2D_H__
#define __PIONNER_GFX_STRUCT_GEOMETRY2D_H__

#include "asset/Asset.h"

namespace pio
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	constexpr static const uint32_t LINE_MESH_LIMIT = 65535u;

	struct LineVertex
	{
		glm::vec3 Position{ 0.f };
		glm::vec4 Color{ 0.f, 0.f, 0.f, 1.f };

		LineVertex() {}
		LineVertex(const glm::vec3 &pos, const glm::vec4 &color) : Position(pos), Color(color) {}
	};

	struct QuadVertex
	{
		glm::vec3 Position{ 0.f };
		glm::vec2 Texcoord{ 0.f, 0.f};

		QuadVertex() {}
		QuadVertex(const glm::vec3 &pos, const glm::vec2 &texcoord) : Position(pos), Texcoord(texcoord) {}
	};

	class Geometry2D : public Asset
	{
	public:
		Geometry2D(const std::string &name) : Asset(name) {}
		virtual ~Geometry2D() = default;

		virtual void clear() { Size = Capacity = 0; };

	public:
		uint32_t Size{ 0 };
		uint32_t Capacity{ 0 };

		Ref<VertexArray> VertexArray{};
		Ref<VertexBuffer> VertexBuffer{};
		Ref<IndexBuffer> IndexBuffer{};
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