#pragma once

#include "Importer.h"

struct aiScene;
struct aiNode;

namespace pio
{
	class StaticMesh;
	class SkinnedMesh;
	class Skeleton;
	struct SubMesh;
	struct SubMeshNode;

	class MeshImporter : public Importer
	{
		RTTR_ENABLE(Importer)
	public:
		MeshImporter(const ImportParams& params);
		virtual ~MeshImporter() = default;

		virtual Ref<Asset> Load() override;

	private:
		Ref<Asset> LoadFromSubMesh();
		Ref<Asset> LoadFromDisk();

		bool HasAnyBone(const aiScene* scene) const;
		Ref<Asset> ParseStaticMesh(const aiScene* scene);
		Ref<Asset> ParseSkinnedMesh(const aiScene* scene);
		void ParseMesh(const aiScene* scene, const Ref<StaticMesh>& mesh);
		void ParseMaterial(const aiScene* scene, const Ref<StaticMesh>& mesh);
		void ParseRelation(const aiScene* scene, const Ref<StaticMesh>& mesh);
		void ParseSkeleton(const aiScene* scene, const Ref<SkinnedMesh>& mesh);
		void ParseAnimationClip(const aiScene* scene, const Ref<SkinnedMesh>& mesh);
		void ParseBoneInfluence(const aiScene* scene, const Ref<SkinnedMesh>& mesh);
		void TraverseNode(aiNode* aNode, SubMeshNode& sNode,
						  std::vector<SubMesh>& subMeshes,
						  const glm::mat4& parentTransform,
						  uint32_t level);
		void TraverseJoint(const aiNode* root, const std::map<std::string, glm::mat4>& bindInfo,
						   const Ref<Skeleton>& skeleton);
	};
}