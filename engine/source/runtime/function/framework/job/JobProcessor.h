#ifndef __JOB_PROCESSOR_H__
#define __JOB_PROCESSOR_H__

#include <memory>
#include "function/framework/job/JobDef.h"

namespace Pionner
{
	class JobTask;
	class Job;

	class JobProcessor
	{
	public:
		JobProcessor();
		~JobProcessor();

		void addJob(const std::shared_ptr<Job> &job);
		void shutdown();

	private:
		std::shared_ptr<JobTask> m_tasks[JOB_CNT];
	};
}

#endif