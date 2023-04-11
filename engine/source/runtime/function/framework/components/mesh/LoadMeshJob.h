#ifndef __LOAD_MESH_JOB_H__
#define __LOAD_MESH_JOB_H__

#include <vector>
#include <string>
#include "function/framework/job/Job.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace Pionner
{
	struct GameObjPartDesc;
	class RenderEntity;

	class LoadMeshJob : public Job
	{
		friend class MeshComponent;
	public:
		LoadMeshJob(JobType type, JobObserver *ob);
		virtual ~LoadMeshJob();

		virtual void work(JobResult& result) override;

	private:
		void parseObj(const std::string &path, std::shared_ptr<RenderEntity> & root);
		void processNode(aiNode* node, const aiScene* scene, const std::string &rootDir, std::shared_ptr<RenderEntity>& entity);
		void processMesh(int meshIndex, aiMesh* mesh, const aiScene* scene, const std::string& rootDir, std::shared_ptr<RenderEntity>& entity);

		std::vector<GameObjPartDesc> m_meshToLoad;
	};
}

#endif