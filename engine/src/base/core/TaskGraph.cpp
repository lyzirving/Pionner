#include "TaskGraph.h"
#include "TaskSet.h"

namespace pio
{
	PIO_SINGLETON_IMPL(TaskGraph)

	void TaskGraph::Init(const InitParam& param)
	{
		TaskGraph::Get()->Start();
	}

	void TaskGraph::Shutdown()
	{
		TaskGraph::Get()->Stop();
	}

	Ref<WorkTask> TaskGraph::CreateWorkTask(TaskFunc&& fun)
	{
		auto task = CreateRef<WorkTask>(std::move(fun));
		m_TaskSet->PushBack(task);
		return task;
	}

	Ref<PWorkTask> TaskGraph::CreatePromiseTask(PTaskFunc&& fun)
	{
		auto task = CreateRef<PWorkTask>(std::move(fun));
		m_TaskSet->PushBack(task);
		return task;
	}

	Ref<PWorkTask> TaskGraph::CreatePromiseTask(PTaskFunc&& fun, PTaskCb&& cb)
	{
		auto task = CreateRef<PWorkTask>(std::move(fun), std::move(cb));
		m_TaskSet->PushBack(task);
		return task;
	}

	void TaskGraph::Start()
	{
		m_TaskSet = CreateRef<TaskSet>(std::min(m_CoreNum, 5U));
		m_TaskSet->Start();
	}

	void TaskGraph::Stop()
	{
		m_TaskSet->Stop();
	}
}