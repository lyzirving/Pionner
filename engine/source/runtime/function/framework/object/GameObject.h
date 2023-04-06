#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <cstdint>
#include <string>
#include <atomic>
#include <vector>
#include <memory>

namespace Pionner
{
	class Component;

	class GameObject
	{
	public:
		GameObject(const char* url = nullptr);
		virtual ~GameObject();

		inline uint32_t getId() const { return m_id; }
		inline const std::string getName() const { return m_name; }

		void tick(float delta);

	protected:
		static std::atomic<uint32_t> g_objectId;

		uint32_t m_id;
		std::string m_name;
		std::string m_resUrl;

		std::vector<std::shared_ptr<Component>> m_components;
	};
}

#endif