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
	RuntimeLayer::RuntimeLayer(const WindowLayoutParams &param) : Layer(param, "Runtime Layer")
	{
	}

	RuntimeLayer::~RuntimeLayer() = default;

	void RuntimeLayer::onAttach()
	{	
		float aspectRatio = (m_layoutParam.Percentage.Right - m_layoutParam.Percentage.Left) / (m_layoutParam.Percentage.Bottom - m_layoutParam.Percentage.Top);
		uint32_t w = Application::MainWindow()->getWidth();
		uint32_t h = Application::MainWindow()->getHeight();
		m_layoutParam.calculate(w, h);		

		m_scene = CreateRef<Scene>();
		m_renderer = CreateRef<SceneRenderer>();							

		m_scene->setLayoutParam(m_layoutParam);
		m_scene->onAttach(m_renderer);

		const LayoutViewport &vp = m_layoutParam.Viewport;
		m_scene->setCameraViewport(vp.X, vp.Y, vp.Width, vp.Height);	
	}

	void RuntimeLayer::onDetach()
	{
		m_scene->onDetach(m_renderer);

		m_renderer.reset();
		m_scene.reset();
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
		m_scene->setLayoutParam(m_layoutParam);
	}
}