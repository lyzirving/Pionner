#include "RuntimeLayer.h"

#include "Application.h"

#include "window/event/MouseEvent.h"

#include "scene/Scene.h"

#include "gfx/renderer/SceneRenderer.h"
#include "gfx/renderer/Renderer.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/RenderPass.h"
#include "gfx/struct/MeshFactory.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RuntimeLayer"

namespace pio
{
	RuntimeLayer::RuntimeLayer(const LayoutParams &param) : Layer(param, "Runtime Layer")
	{
	}

	RuntimeLayer::~RuntimeLayer() = default;

	void RuntimeLayer::onAttach()
	{	
		m_scene = CreateRef<Scene>();
		m_renderer = CreateRef<SceneRenderer>();
		m_scene->onAttach(m_renderer);

		onWindowSizeChange(Application::MainWindow()->getWidth(),
						   Application::MainWindow()->getHeight());
	}

	void RuntimeLayer::onDetach()
	{
		m_scene->onDetach(m_renderer);

		m_renderer.reset();
		m_scene.reset();
	}

	bool RuntimeLayer::onEvent(Event &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<MouseButtonPressedEvent>(PIO_BIND_EVT_FN(RuntimeLayer::onMouseButtonPressed));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseMovedEvent>(PIO_BIND_EVT_FN(RuntimeLayer::onMouseMoved));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseButtonReleasedEvent>(PIO_BIND_EVT_FN(RuntimeLayer::onMouseButtonReleased));
		if (event.Handled) return true;

		return false;
	}

	void RuntimeLayer::onRenderDestroy()
	{
		m_renderer->onRenderDestroy(*m_scene);
	}

	void RuntimeLayer::onUpdate(const Timestep &ts)
	{
		m_scene->onUpdate(ts);
		m_scene->onRender(m_renderer, ts);			
	}

	void RuntimeLayer::onWindowSizeChange(uint32_t width, uint32_t height)
	{		
		m_layoutParam.calculate(width, height);

		const LayoutViewport &vp = m_layoutParam.Viewport;
		m_scene->setCameraViewport(vp.X, vp.Y, vp.Width, vp.Height);

		if (m_screenQuad == NullAsset)
		{
			LayoutRect &rect = m_layoutParam.Position;
			m_screenQuad = MeshFactory::CreateScreenQuad(rect.Left, rect.Top, rect.Right, rect.Bottom, width, height)->getHandle();
			m_scene->setScreenQuad(m_screenQuad);
		}
	}

	bool RuntimeLayer::onMouseButtonPressed(Event &event)
	{
		glm::vec2 cursor = Application::MainWindow()->getCursorPos();
		const LayoutRect &rect = m_layoutParam.Position;
		if (rect.contain(cursor.x, cursor.y))
		{
			m_eventState.ButtonPressed = true;
			m_eventState.PressedTime = TimeUtil::currentTimeMs();
			m_scene->onMouseButtonPressed(event);
		}
		return m_eventState.ButtonPressed;
	}

	bool RuntimeLayer::onMouseMoved(Event &event)
	{
		m_scene->onMouseMoved(event);
		return m_eventState.ButtonPressed;
	}

	bool RuntimeLayer::onMouseButtonReleased(Event &event)
	{
		if (m_eventState.ButtonPressed)
		{
			m_eventState.ButtonPressed = false;			
			if (!m_scene->onMouseButtonReleased(event) && UIEventTracker::IsClick(TimeUtil::currentTimeMs(), m_eventState.PressedTime))
			{
				onHandleClick(Application::MainWindow()->getCursorPos());
			}
			return true;
		}
		return false;
	}

	bool RuntimeLayer::onHandleClick(const glm::vec2 &cursor)
	{		
		return m_scene->dispatchClick(UiDef::ScreenToViewport(cursor, m_layoutParam));
	}
}