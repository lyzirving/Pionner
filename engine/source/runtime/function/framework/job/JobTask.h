#ifndef __JOB_TASK_H__
#define __JOB_TASK_H__

#include <atomic>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "function/framework/job/JobDef.h"

namespace Pionner
{
	class Job;

	enum JobTaskState : uint8_t
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_END
	};

	class JobTask
	{
	public:
		JobTask(JobType type);
		virtual ~JobTask();

		inline JobType getType() { return m_type; }

		void addJob(const std::shared_ptr<Job>& job);
		void quit();

	protected:
		void workLoop();

	private:
		std::vector<std::shared_ptr<Job>> m_jobs;
		std::mutex                        m_mutex;
		std::condition_variable           m_jobCond;
		std::atomic<bool>                 m_requestQuit;
		std::thread                       m_thread;
		volatile JobTaskState             m_state;
		JobType                           m_type;
	};
}

#endif