#pragma once

#include "WorkTask.h"
#include "base/core/Thread.h"

namespace pio
{
	namespace TaskStatus
	{
		enum : TStatus
		{
			Idle,
			Started,
			RequestQuit
		};
	}

	class TaskSet
	{
	public:
		TaskSet(uint32_t num);
		~TaskSet();

		void PushBack(const Ref<WorkTask>& task);
		void Start();
		void Stop();		
		void DispatchWork();

		bool IsQuit() const { return m_Status != TaskStatus::Started; }

	private:
		std::mutex m_TaskMutex;
		std::condition_variable m_NotEmptyCond;
		std::list<Ref<WorkTask>> m_Tasks;
		
		std::vector<Ref<Thread>> m_Threads;
		std::atomic<TStatus> m_Status{ TaskStatus::Idle };
	};
}