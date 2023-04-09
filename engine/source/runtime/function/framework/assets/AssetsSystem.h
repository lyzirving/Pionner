#ifndef __ASSETS_MGR_H__
#define __ASSETS_MGR_H__

#include <memory>

namespace Pionner
{
	class JobProcessor;

	class AssetsSystem
	{
	public:
		AssetsSystem();
		~AssetsSystem();

		void initialize();
		void shutdown();

	private:
		std::shared_ptr<JobProcessor> m_processor;
	};
}

#endif