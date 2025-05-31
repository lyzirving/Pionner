#pragma once

#include "Object.h"

namespace pio
{
	class TaskRet : public Object
	{
		RTTR_ENABLE(Object)
	public:
		TaskRet() {}
		virtual ~TaskRet() = default;
	};

	using WorkRet = Ref<TaskRet>;

	using TaskFunc = std::function<void()>;
	using PTaskFunc = std::function<WorkRet()>;
	using PTaskCb = std::function<void(const WorkRet&)>;
}