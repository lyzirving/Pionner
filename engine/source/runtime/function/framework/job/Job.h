#ifndef __JOB_H__
#define __JOB_H__

#include <memory>
#include <vector>

#include "function/framework/job/JobDef.h"

namespace Pionner
{
	class RenderEntity;

	struct JobResult
	{
		std::vector<std::shared_ptr<RenderEntity>> entities;
	};

	class JobObserver
	{
	public:
		JobObserver() {}
		virtual ~JobObserver() = default;

		virtual void onJobEnd(JobResult& result) = 0;
	};

	class Job
	{
		friend class JobTask;
	public:
		Job(JobType type, JobObserver *ob)
			: m_type(type)
			, m_ob(ob)
		{
		}

		virtual ~Job() = default;

		inline JobType getType() { return m_type; }

		virtual void work(JobResult &result) = 0;

	protected:
		JobObserver *m_ob;
		JobType     m_type;
	};
}

#endif