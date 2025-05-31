#pragma once

#include "TaskDef.h"

namespace pio
{
	/*
	* @brief Task with none return value
	*/
	class WorkTask
	{
	public:
		WorkTask() {}
		WorkTask(TaskFunc&& func);
		virtual ~WorkTask() = default;
		
		virtual bool IsOver() const { return !m_Future.valid(); }
		void Wait();
		bool Wait(uint64_t timeoutMs);

	protected:
		virtual void DoWork();

	private:
		TaskFunc m_Func;
		std::promise<void> m_Barrier;
		std::future<void> m_Future;

	private:
		friend class TaskSet;
	};

	/*
	* @brief PromiseWorkTask which has return value
	*/
	class PWorkTask : public WorkTask
	{
	public:		
		PWorkTask(PTaskFunc&& func);
		PWorkTask(PTaskFunc&& func, PTaskCb&& cb);
		~PWorkTask() = default;
		
		virtual bool IsOver() const override { return !m_PFuture.valid(); }
		WorkRet Wait();
		std::pair<bool, WorkRet> Wait(uint64_t timeoutMs);

	protected:
		virtual void DoWork() override;

	private:
		PTaskFunc m_PFunc;
		PTaskCb m_Cb;
		std::promise<WorkRet> m_PBarrier;
		std::future<WorkRet> m_PFuture;

	private:
		friend class TaskSet;
	};
}