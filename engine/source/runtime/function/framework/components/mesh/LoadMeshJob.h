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

	class LoadMeshJob : public Job
	{
		friend class MeshComponent;
	public:
		LoadMeshJob(JobType type, JobObserver *ob);
		virtual ~LoadMeshJob();

		virtual void work(JobResult& result) override;

	private:
		void parseObj(const std::string &path);
		void processNode(aiNode* node, const aiScene* scene);
		void processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<GameObjPartDesc> m_meshToLoad;
	};
}

#endif