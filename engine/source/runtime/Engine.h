#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <chrono>

namespace Pionner
{
	class Engine
	{
	public:
		Engine();
		virtual ~Engine();

		void initialize();
		void shutdown();

		float calculateDeltaTime();
		bool tickFrame(float deltaTime);

	protected:
		static const float s_fpsAlpha;

		void calculateFps(float deltaTime);

		void tickLogic(float deltaTime);
		void tickRender(float deltaTime);

	private:
		std::chrono::steady_clock::time_point m_lastTickTimePoint;
		float m_averageDuration;
		int m_frameCnt, m_fps;
	};
}

#endif // !__ENGINE_H__
