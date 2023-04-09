#include <cmath>
#include <bitset>

#include "function/framework/job/JobProcessor.h"
#include "function/framework/job/JobTask.h"
#include "function/framework/job/Job.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "JobProcessor"

namespace Pionner
{
	JobProcessor::JobProcessor()
	{
		std::bitset<JOB_CNT> tasks;
		tasks.reset();
		tasks.set(JOB_LOAD_ASSETS);

		for (uint8_t i = 0; i < JOB_CNT; i++)
		{
			if (tasks.test(i))
			{
				m_tasks[i] = std::shared_ptr<JobTask>(new JobTask(JobType(i)));
			}
		}
	}

	JobProcessor::~JobProcessor()
	{
		
	}

	void JobProcessor::addJob(const std::shared_ptr<Job>& job)
	{
		JobType type = job->getType();
		if (m_tasks[type])
		{
			m_tasks[type]->addJob(job);
		}
		else
		{
			LOG_ERR("task[%d] is invalid", type);
		}
	}

	void JobProcessor::shutdown()
	{
		for (uint8_t i = 0; i < JOB_CNT; i++)
		{
			if (m_tasks[i])
			{
				m_tasks[i]->quit();
			}
		}
	}
}