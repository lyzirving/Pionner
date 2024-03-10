#include "MotionController.h"

namespace pio
{
	PIO_SINGLETON_IMPL(MotionController)

	MotionController::MotionController()
	{
	}

	MotionController::~MotionController() = default;

	void MotionController::Init()
	{
		MotionController::Get();		
	}

	void MotionController::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		delete s_instance;
		s_instance = nullptr;
	}
}