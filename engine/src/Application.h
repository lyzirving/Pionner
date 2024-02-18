#ifndef __PIONNER_APPLICATION_H__
#define __PIONNER_APPLICATION_H__

#include "window/Window.h"

#include "gfx/GraphicsContext.h"
#include "gfx/renderer/RenderThread.h"
#include "gfx/renderer/Renderer.h"

#include "ui/layer/LayerManager.h"

namespace pio
{
	class Application : public RenderListener
	{
	public:
		Application();
		virtual ~Application();

		virtual void onInit();
		virtual void onQuit();

		virtual void onRenderInit() override;
		virtual void onRenderDestroy() override;

	public:
		void onEvent(Event &event);
		bool onWindowClose(Event &event);
		bool onWindowResize(Event &event);

		void run();

	public:
		static Application *Get()        { return s_app; }
		static Window *MainWindow() { return s_app->m_window.get(); }
		static GraphicsContext *GfxContext() { return s_app->m_graphics.get(); }

	private:
		static Application *s_app;

		struct FrameRecorder
		{
			const uint32_t RoomSpeed;
			const uint32_t NumSeconds;
			const uint32_t ArraySize;

			FrameRecorder(uint32_t roomSpeed = 60, uint32_t numSeconds = 5) 
				: RoomSpeed(roomSpeed), NumSeconds(numSeconds), ArraySize(roomSpeed * numSeconds)
			{ 
				FpsArray.resize(ArraySize, 0.f);
			}

			float avgFps(uint64_t deltaMs)
			{
				uint32_t ind = FramePassed;
				float divisor = float(deltaMs) / 1000.f;
				float realFps{ 0.f };
				if (!Math::IsZero(divisor))
				{
					realFps = 1.f / divisor;
				}

				MovingSum -= FpsArray[ind];
				FpsArray[ind] = realFps;
				MovingSum += realFps;

				FramePassed = (FramePassed + 1) % ArraySize;

				return MovingSum / (float)ArraySize;
			}

		private:
			uint32_t FramePassed{ 0 };
			float MovingSum{ 0.f };
			std::vector<float> FpsArray;
		};

	public:
		std::mutex EventMutex;

	private:
		bool m_running{ true };
		uint64_t m_lastFrameTimeMs{ 0 };

		RenderThread  m_renderThread{};
		Scope<Window> m_window{ nullptr };
		Scope<GraphicsContext> m_graphics{ nullptr };

		LayerManager m_layerManager{};
		FrameRecorder m_frameRec{60, 5};
	};
}

#endif