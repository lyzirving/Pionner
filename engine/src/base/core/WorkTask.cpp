#include "WorkTask.h"

#include "base/utils/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "WorkTask"

namespace pio
{
	// ----------------------- WorkTask Begin -----------------------
	WorkTask::WorkTask(TaskFunc&& func) : m_Func(std::move(func))
	{
		m_Future = m_Barrier.get_future();
	}

	void WorkTask::Wait()
	{
		if(IsOver())
			return;

		m_Future.wait();
		//Make it over
		m_Future.get();
	}

	bool WorkTask::Wait(uint64_t timeoutMs)
	{
		if(IsOver())
			return false;

		std::chrono::milliseconds span(timeoutMs);
		auto status = m_Future.wait_for(span);
		//Make it over
		m_Future.get();
		return status != std::future_status::timeout;
	}

	void WorkTask::DoWork()
	{
		if(m_Func)
		{
			m_Func();
			m_Barrier.set_value();
		}
		else
		{			
			LOGE("task doesn't contain a target");
		}
	}
	// ----------------------- WorkTask End -----------------------

	// ----------------------- PWorkTask Begin -----------------------
	PWorkTask::PWorkTask(PTaskFunc&& func) : WorkTask(), m_PFunc(std::move(func))
	{
		m_PFuture = m_PBarrier.get_future();
	}

	PWorkTask::PWorkTask(PTaskFunc&& func, PTaskCb&& cb) : WorkTask(), 
		m_PFunc(std::move(func)), m_Cb(std::move(cb))
	{
		m_PFuture = m_PBarrier.get_future();
	}

	void PWorkTask::DoWork()
	{
		if(m_PFunc)
		{
			auto ret = m_PFunc(); 
			if(m_Cb)
			{
				m_Cb(ret);
			}
			m_PBarrier.set_value(ret);
		}
		else
		{
			LOGE("task doesn't contain a target");
		}
	}

	WorkRet PWorkTask::Wait()
	{
		if(IsOver())
			return WorkRet();

		m_PFuture.wait();
		//Make it over
		return m_PFuture.get();
	}

	std::pair<bool, WorkRet> PWorkTask::Wait(uint64_t timeoutMs)
	{		
		if(IsOver())
			return std::make_pair(false, WorkRet{});

		std::chrono::milliseconds span(timeoutMs);
		auto status = m_PFuture.wait_for(span);
		//Make it over
		return std::make_pair(status != std::future_status::timeout, 
							  m_PFuture.get());
	}
}