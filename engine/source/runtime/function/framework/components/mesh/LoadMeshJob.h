#ifndef __LOAD_MESH_JOB_H__
#define __LOAD_MESH_JOB_H__

#include <vector>
#include "function/framework/job/Job.h"

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
		std::vector<GameObjPartDesc> m_meshToLoad;
	};
}

#endif