#include "function/framework/job/JobTask.h"
#include "function/framework/job/Job.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "JobTask"

namespace Pionner
{
	JobTask::JobTask(JobType type)
		: m_jobs()
		, m_mutex(), m_jobCond()
		, m_requestQuit(false)
		, m_thread()
		, m_state(STATE_IDLE)
		, m_type(type)
	{
		m_thread = std::thread([this]() { workLoop(); });
	}

	JobTask::~JobTask()
	{
		auto itr = m_jobs.begin();
		while (itr != m_jobs.end())
		{
			itr->reset();
			itr = m_jobs.erase(itr);
		}
	}

	void JobTask::workLoop()
	{
		LOG_DEBUG("task[%d] enter loop", m_type);
		m_state = STATE_RUN;
		while (true)
		{
			if (m_requestQuit.load())
				break;

			std::shared_ptr<Job> job;
			{
				std::unique_lock<std::mutex> lock{ m_mutex };
				if (m_jobs.empty())
					m_jobCond.wait(lock);

				// in case of spurious awakenings 
				if (!m_jobs.empty())
				{
					job = m_jobs.back();
					m_jobs.pop_back();
				}
			}
			if (job)
			{
				job->work();
				if (job->m_observer) job->m_observer->onJobEnd();
			}
		}
		m_state = STATE_END;
		LOG_DEBUG("task[%d] quit loop", m_type);
	}

	void JobTask::addJob(const std::shared_ptr<Job>& job)
	{
		{
			std::unique_lock<std::mutex> lock{ m_mutex };
			m_jobs.push_back(job);
		}
		m_jobCond.notify_one();
	}

	void JobTask::quit()
	{
		m_requestQuit.store(true);
		if (m_state != STATE_END && m_thread.joinable())
		{
			m_jobCond.notify_one();
			m_thread.join();
		}
	}
}
