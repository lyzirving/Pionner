#include "function/framework/components/mesh/LoadMeshJob.h"
#include "function/framework/object/GameObjectDef.h"

namespace Pionner
{
	LoadMeshJob::LoadMeshJob(JobType type, JobObserver* ob)
		: Job(type, ob)
		, m_meshToLoad()
	{
	}

	LoadMeshJob::~LoadMeshJob()
	{
		m_ob = nullptr;
	}

	void LoadMeshJob::work(JobResult& result)
	{

	}
}