#ifndef __WORLD_MGR_H__
#define __WORLD_MGR_H__

#include <string>
#include <map>
#include <memory>

namespace Pionner
{
	class Level;

	class WorldMgr
	{
	public:
		WorldMgr();
		~WorldMgr();

		void initialize();
		void shutdown();
		void tick(float delta);

	private:
		std::map<std::string, std::shared_ptr<Level>> m_levels;
		std::shared_ptr<Level> m_activeLevel;
	};
}

#endif