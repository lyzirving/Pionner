#ifndef __PIONNER_ASSET_ASSIMP_MESH_IMPORTER_H__
#define __PIONNER_ASSET_ASSIMP_MESH_IMPORTER_H__

#include "asset/Asset.h"

struct aiScene;

namespace pio
{
	class MeshSource;

	class AssimpMeshImporter
	{
	public:
		AssimpMeshImporter(const std::string &name, AssetFmt fmt);
		~AssimpMeshImporter();

		Ref<Asset> importToMeshSource();

		inline const std::string &getAssetDir() const { return m_assetsDir; }

	private:
		void parseSkeleton(const aiScene *scene, Ref<MeshSource> &meshSource);
		void parseAnimation(const aiScene *scene, Ref<MeshSource> &meshSource);
		void parseMeshes(const aiScene *scene, Ref<MeshSource> &meshSource);
		void parseBoneInfluence(const aiScene *scene, Ref<MeshSource> &meshSource);
		void parseMaterial(const aiScene *scene, Ref<MeshSource> &meshSource);
		void createBuffers(const aiScene *scene, Ref<MeshSource> &meshSource);
		void createGPUSkinning(Ref<MeshSource> &meshSource);

		// build node hierarchy for meshes
		void traverseNodes(Ref<MeshSource> &meshSource, void *assimpNode, uint32_t nodeIndex, 
						   const glm::mat4 &parentTransform = glm::mat4(1.0f), 
						   uint32_t level = 0);

	private:
		const std::string m_name;
		const std::string m_assetsDir;
		const AssetFmt m_fmt;
	};
}

#endif
