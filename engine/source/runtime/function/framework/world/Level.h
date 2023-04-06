#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <unordered_map>
#include <memory>
#include <string>

namespace Pionner
{
	class GameObject;

	class Level
	{
	public:
		Level(const char *url = "default");
		~Level();

		void initialize();
		void shutdown();
		void tick(float delta);

	private:
		std::unordered_map<uint32_t, std::shared_ptr<GameObject>> m_objects;
		std::string m_levelResUrl;
	};
}

#endif