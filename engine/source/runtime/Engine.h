#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <cstdint>

namespace pio
{
	class Engine
	{
	public:
		Engine();
		virtual ~Engine();

		void initialize();
		void shutdown();

		uint64_t calculateDeltaTime();
		bool     tickFrame(uint64_t deltaMs);

	protected:

		void tickLogic(uint64_t deltaMs);
		void tickRender(uint64_t deltaMs);

	private:
		uint64_t m_lastTimeMs{ 0 };
	};
}

#endif // !__ENGINE_H__
