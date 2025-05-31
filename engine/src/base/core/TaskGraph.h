#pragma once

#include "WorkTask.h"

namespace pio
{
	class TaskSet;

	class TaskGraph
	{
		PIO_SINGLETON_DECLARE(TaskGraph)

	public:
		Ref<WorkTask> CreateWorkTask(TaskFunc&& fun);
		Ref<PWorkTask> CreatePromiseTask(PTaskFunc&& fun);
		Ref<PWorkTask> CreatePromiseTask(PTaskFunc&& fun, PTaskCb&& cb);

		void Start();
		void Stop();
		void SetCoreNum(uint32_t num) { m_CoreNum = num; }	

	private:
		uint32_t m_CoreNum{ 2 };
		Ref<TaskSet> m_TaskSet;
	};
}