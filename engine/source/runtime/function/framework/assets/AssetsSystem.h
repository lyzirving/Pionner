#ifndef __ASSETS_MGR_H__
#define __ASSETS_MGR_H__

#include <memory>

namespace Pionner
{
	class JobProcessor;
	class Job;

	class AssetsSystem
	{
	public:
		AssetsSystem();
		~AssetsSystem();

		void initialize();
		void shutdown();

		void addJob(const std::shared_ptr<Job>& job);

	private:
		std::shared_ptr<JobProcessor> m_processor;
	};
}

#endif