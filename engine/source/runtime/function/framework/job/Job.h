#ifndef __JOB_H__
#define __JOB_H__

#include "function/framework/job/JobDef.h"

namespace Pionner
{
	class JobObserver
	{
	public:
		JobObserver() {}
		virtual ~JobObserver() = default;

		virtual void onJobEnd() = 0;
	};

	class Job
	{
		friend class JobTask;
	public:
		Job(JobType type, JobObserver *observer = nullptr) 
			: m_type(type)
			, m_observer(observer) 
		{
		}

		virtual ~Job() = default;

		inline JobType getType() { return m_type; }

		virtual void work() = 0;

	private:
		JobObserver* m_observer;
		JobType      m_type;
	};
}

#endif