#include "function/framework/assets/AssetsSystem.h"
#include "function/framework/job/JobProcessor.h"

namespace Pionner
{
	AssetsSystem::AssetsSystem() : m_processor(nullptr)
	{
	}

	AssetsSystem::~AssetsSystem()
	{
		m_processor.reset();
	}

	void AssetsSystem::initialize()
	{
		m_processor = std::make_shared<JobProcessor>();
	}

	void AssetsSystem::shutdown()
	{
		m_processor->shutdown();
	}

	void AssetsSystem::addJob(const std::shared_ptr<Job>& job)
	{
		m_processor->addJob(job);
	}
}