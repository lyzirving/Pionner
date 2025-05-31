#include "TaskSet.h"

#include "base/utils/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "TaskSet"

namespace pio
{
	TaskSet::TaskSet(uint32_t num)
	{
		m_Threads.resize(num);
	}

	TaskSet::~TaskSet()
	{
	}

	void TaskSet::PushBack(const Ref<WorkTask>& task)
	{
		if(IsQuit())
			return;

		{
			std::lock_guard<std::mutex> lk{ m_TaskMutex };
			m_Tasks.push_back(task);
		}
		// Manual unlocking is done before notifying, to avoid waking up
		// the waiting thread only to block again (see notify_one for details)
		m_NotEmptyCond.notify_all();
	}

	void TaskSet::Start()
	{
		if(m_Status == TaskStatus::Started)
			return;

		m_Status = TaskStatus::Started;

		for(size_t i = 0; i < m_Threads.size(); i++)
		{			
			m_Threads[i] = Thread::Create(std::string("WorkThread-").append(std::to_string(i)));
			m_Threads[i]->Bind(std::bind(&TaskSet::DispatchWork, this));
			m_Threads[i]->Start();
		}		
	}

	void TaskSet::Stop()
	{
		if(m_Status != TaskStatus::Started)
			return;

		LOGD("begin to quit");
		m_Status = TaskStatus::RequestQuit;
		m_NotEmptyCond.notify_all();
		for(size_t i = 0; i < m_Threads.size(); i++)
		{
			m_Threads[i]->Stop();
		}
		LOGD("stop finishes");
		m_Status = TaskStatus::Idle;
	}	

	void TaskSet::DispatchWork()
	{		
		Ref<WorkTask> work;
		while(m_Status == TaskStatus::Started)
		{
			{
				std::unique_lock<std::mutex> lk{ m_TaskMutex };
				m_NotEmptyCond.wait(lk, [this]()
				{
					return !m_Tasks.empty() || m_Status == TaskStatus::RequestQuit;
				});

				if(m_Status == TaskStatus::RequestQuit)
				{
					m_Tasks.clear();
					break;
				}

				if(!m_Tasks.empty())
				{
					work = m_Tasks.front();
					m_Tasks.pop_front();					
				}
			}

			if(work)
			{
				work->DoWork();			
			}
		}
	}
}