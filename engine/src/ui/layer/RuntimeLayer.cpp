#include "RuntimeLayer.h"

#include "Application.h"

#include "asset/AssetsManager.h"
#include "event/MouseEvent.h"

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
		float aspectRatio = (m_layoutParam.Ratio.Right - m_layoutParam.Ratio.Left) / (m_layoutParam.Ratio.Bottom - m_layoutParam.Ratio.Top);
		uint32_t w = Application::MainWindow()->width();
		uint32_t h = Application::MainWindow()->height();
		m_layoutParam.calculate(w, h);		
		
		m_scene = AssetsManager::Get()->CreateRuntimeAssets<Scene>(true);
		m_renderer = CreateRef<SceneRenderer>();							

		m_scene->onAttach(m_renderer, m_layoutParam);
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

		const Viewport &vp = m_layoutParam.Viewport;
		m_scene->setViewport(vp.x(), vp.y(), vp.w(), vp.h());
	}
}