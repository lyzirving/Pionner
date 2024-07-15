#ifndef __PIONNER_GFX_STRUCT_MESH_H__
#define __PIONNER_GFX_STRUCT_MESH_H__

#include <unordered_map>

#include "asset/Asset.h"
#include "animation/AnimDef.h"

#include "core/math/AABB.h"
#include "core/math/Transform.h"

#include "gfx/rhi/Uniform.h"
#include "scene/SceneDef.h"

namespace pio
{
	class Mesh;
	class Skeleton;
	class Material;
	class Animation;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class MaterialTable;
	class UniformBuffer;
	class Entity;

	// ----------------------- constants related with Mesh --------------------------
	static const uint32_t ROOT_NODE_INDEX = 0xffffffff;
	// ------------------------------------------------------------------------------

	// ----------------------- structures related with Mesh -------------------------

	struct MeshNode
	{
		uint32_t Parent{ ROOT_NODE_INDEX }; // parent index
		std::vector<uint32_t> Children;     // child nodes
		std::vector<uint32_t> Submeshes;    // index of submeshes

		std::string Name{"Root"};
		/*
		* LocalTransform is a transformation relative to the node's parent.
		* If node is root, its parent is world's origin(0, 0, 0)
		*/
		glm::mat4 LocalTransform{1.f};
		uint32_t Level{ 0 };

		inline bool isRoot() const { return Parent == ROOT_NODE_INDEX; }
	};

	struct BoneInfluence
	{
		// Max num of influence for a vertex is 4
		float Weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		uint32_t BoneInfoIndices[PIO_MAX_BONE_INFLUENCE] = { NullIndex, NullIndex, NullIndex, NullIndex };

		void normalizeWeights();
	};

	struct Submesh
	{
	public:
		AABB BoundingBox{};

		uint32_t VertexOffset{ 0 };
		uint32_t IndexOffset{ 0 };
		uint32_t MaterialIndex{ NullIndex };
		uint32_t VertexCount{ 0 };
		uint32_t IndexCount{ 0 };

		// World transform, which equals transform passed from root node
		glm::mat4 Transform{ 1.0f };      
		// Transform belonging to the mesh
		glm::mat4 LocalTransform{ 1.0f }; 

		std::string NodeName{};
		std::string MeshName{};
		// Rigging means whether the sumesh is binded to bones
		bool IsRigged{ false };

		// The size of BoneInfluences is equal to VertexCount
		// Which means each vertex has its corresponding bone influence.
		std::vector<BoneInfluence> BoneInfluences;

		Ref<VertexArray>  VertexArray{};
		Ref<VertexBuffer> BoneInfluenceBuffer{};
		Ref<VertexBuffer> VertexBuffer{};
		Ref<IndexBuffer>  IndexBuffer{};

		Ref<Entity> Ent;
	};

	struct BoneInfo
	{
		glm::mat4 SubMeshInverseTransform{ 1.f };
		glm::mat4 InverseBindPose{ 1.f };
		uint32_t SubMeshIndex{ 0 };
		uint32_t BoneIndex{ 0 };

		BoneInfo() = default;
		BoneInfo(glm::mat4 subMeshInverseTransform, glm::mat4 inverseBindPose, uint32_t subMeshIndex, uint32_t boneIndex)
			: SubMeshInverseTransform(subMeshInverseTransform), InverseBindPose(inverseBindPose)
			, SubMeshIndex(subMeshIndex), BoneIndex(boneIndex)
		{
		}
	};	

	struct BoneTransformUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(BoneTransformUD)

		// ---------- Uniform Data -------------		
		glm::mat4 Transform[PIO_MAX_BONE_MATRIX];
		// -------------------------------------

		// ---------- Non Uniform -------------
		GPUSkinning GPUTransform{};
		bool GPUAnimated{ false };
		// ------------------------------------

		BoneTransformUD() { setDefault(); }

		glm::mat4 &operator[](uint32_t index) { return Transform[index]; }
		void setDefault() 
		{ 
			for (uint32_t i = 0; i < PIO_MAX_BONE_MATRIX; i++) 
				Transform[i] = glm::mat4(1.f);

			GPUTransform.CurrentFrame = 0;
		}
	};
	// -------------------------------------------------------------------------------

	// ------------------------- classes related with Mesh ---------------------------
	class MeshSource : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::MeshSource)
	public:
		MeshSource(const std::string &name);
		virtual ~MeshSource();

	public:
		bool hasSkeleton() const { return m_skeleton.get() != nullptr; }
		bool hasAnimation() const { return !m_animations.empty(); }
		const std::vector<Submesh> &getSubmeshes() const { return m_submeshes; }
		const std::vector<Ref<Animation>> &getAnimation() const { return m_animations; }
		const std::vector<MeshNode> &getMeshNodes() const { return m_nodes; }
		const Ref<Skeleton> &getSkeleton() const { return m_skeleton; }
		const BoneTransformUD &getBoneTransformUD() const { return m_boneTransform; }
		const Ref<UniformBuffer> &getBoneTransformUB() const { return m_boneTransformUB; }
		const AABB &getAABB() const { return m_boundingBox; }
		uint32_t getSubmeshNum() const { return m_submeshes.size(); }
		uint32_t getAnimationNum() const { return m_animations.size(); }

	public:
		Transform GlobalPose{};

	protected:
		AABB m_boundingBox{};

		std::vector<Submesh> m_submeshes{};
		std::unordered_map<uint32_t, std::vector<Triangle>> m_triangleCache{};

		std::vector<Vertex> m_vertices{};
		std::vector<Index> m_indices{};
		std::vector<MeshNode> m_nodes{};

		Ref<Skeleton> m_skeleton;
		std::vector<Ref<Animation>> m_animations;
		BoneTransformUD m_boneTransform{};
		Ref<UniformBuffer> m_boneTransformUB;

		std::vector<Ref<Material>> m_materials;
	private:
		friend class AssimpMeshImporter;
		friend class MeshFactory;
		friend class Animator;
		friend class Mesh;
		friend class StaticMesh;
		friend class MeshBase;
	};

	class MeshBase : public Asset
	{
	public:
		MeshBase();
		MeshBase(const Ref<MeshSource> &meshSource);
		MeshBase(const Ref<MeshSource> &meshSource, const std::vector<uint32_t> &submeshIndex);
		virtual ~MeshBase();

	public:
		uint32_t getSubmeshNum() const { return m_submeshIndex.size(); }
		const std::vector<uint32_t> &getSubmeshIndex() const { return m_submeshIndex; }
		const Ref<MeshSource> &getMeshSource() const { return m_meshSource; }

		Ref<MaterialTable> &getMaterialTable() { return m_materialTable; }
		const Ref<MaterialTable> &getMaterialTable() const { return m_materialTable; }

	private:
		void createMaterialTable(const std::vector<Ref<Material>> &materials);
		void setSubmeshIndex(const std::vector<uint32_t> &submeshIndex);
		void setMaterial(const std::vector<Ref<Material>> &materials);

	protected:
		Ref<MeshSource> m_meshSource;
		Ref<MaterialTable> m_materialTable;
		std::vector<uint32_t> m_submeshIndex;
	};

	/*
	* Static Mesh is meshes like Sphere, Cube, Cylinder and so on.
	*/
	class StaticMesh : public MeshBase
	{
		OVERRIDE_ASSET_TYPE(AssetType::StaticMesh)
	public:
		StaticMesh(const Ref<MeshSource> &meshSource);
		// Mesh could be sub part of a whole mesh source
		StaticMesh(const Ref<MeshSource> &meshSource, const std::vector<uint32_t> &submeshIndex);
		virtual ~StaticMesh() = default;
	};

	class Mesh : public MeshBase
	{
		OVERRIDE_ASSET_TYPE(AssetType::Mesh)
	public:
		Mesh(const Ref<MeshSource> &meshSource);
		// Mesh could be sub part of a whole mesh source
		Mesh(const Ref<MeshSource> &meshSource, const std::vector<uint32_t> &submeshIndex);
		virtual ~Mesh() = default;

	public:
		inline bool hasSkeleton() { return m_meshSource && m_meshSource->hasSkeleton(); }
	};

	template<>
	bool Asset::is<MeshSource>() const;

	template<>
	bool Asset::is<Mesh>() const;

	template<>
	bool Asset::is<StaticMesh>() const;

	// -------------------------------------------------------------------------------
}

#endif