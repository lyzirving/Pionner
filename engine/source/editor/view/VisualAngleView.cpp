#include "view/VisualAngleView.h"
#include "function/ui/WindowUI.h"

#include "function/render/RenderDef.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/scene/SceneMgr.h"
#include "function/render/scene/Frustum.h"

#include "function/framework/comp/MeshComp.h"
#include "function/event/EventMgr.h"

#include "function/render/resource/ResourceDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VisualAngleView"

namespace Pionner
{
	VisualAngleView::VisualAngleView() : WindowView(ORDER_VIEW_1)
		, m_meshComp(new MeshComp), m_vertexArray(), m_indice()
		, m_renderport()
	{
		m_uid = UID_VISUAL_ANGLE;
		m_meshComp->m_color = glm::vec4(0.392f, 0.392f, 0.392f, 0.6f);
	}

	VisualAngleView::~VisualAngleView()
	{
		m_meshComp.reset();
	}

	void VisualAngleView::draw(RenderParam &param)
	{
		auto evtMgr = param.rhi->getWindowSystem()->getEvtMgr();

		// Decide the view's visibility
		if (!m_layout.contains(evtMgr->getCursorPosX(), evtMgr->getCursorPosY()))
		{
			return;
		}

		init();

		auto rhi = param.rhi;
		auto sceneMgr = param.sceneMgr;
		auto frustum = sceneMgr->m_frustum;
		auto cmd = rhi->getDrawCmd();

		rhi->restoreViewportState();

		rhi->viewportSub(m_renderport.m_left, m_renderport.m_top, m_renderport.m_width, m_renderport.m_height);

		cmd->drawCircle(m_meshComp, param);

		rhi->popViewportState();
	}

	void VisualAngleView::layout(int32_t windowWidth, int32_t windowHeight)
	{
		if (!m_parent)
		{
			LOG_ERR("parent is invalid");
			return;
		}

		std::shared_ptr<WindowView> renderView{ nullptr };
		if (!(renderView = m_parent->getView(UID_RENDER_PORT)))
		{
			LOG_ERR("needed views are invalid");
			return;
		}

		ViewLayout renderLayout = renderView->getLayout();

		m_layout.m_width = std::min(renderLayout.m_width, renderLayout.m_height) / 7;
		m_layout.m_height = m_layout.m_width;

		m_layout.m_left = renderLayout.m_left + renderLayout.m_width - m_layout.m_width - /*extra margin*/m_layout.m_width / 10;
		m_layout.m_top = renderLayout.m_top + /*extra margin*/m_layout.m_width / 10;

		m_renderport.m_width = m_layout.m_width;
		m_renderport.m_height = m_layout.m_height;
		m_renderport.m_left = m_layout.m_left;
		m_renderport.m_top = windowHeight - m_layout.m_top - m_renderport.m_height;

		std::string key{ "circle_radius" };
		float radius = float(m_layout.m_width) * 0.4f;

		auto itr = m_meshComp->m_args.find(key);
		if (itr != m_meshComp->m_args.end())
		{
			itr->second.argFlt = radius;
		}
		else
		{
			CompArg arg;
			arg.argFlt = radius;
			m_meshComp->m_args.insert(std::make_pair(key, arg));
		}

		key = "viewport_size";
		itr = m_meshComp->m_args.find(key);
		if (itr != m_meshComp->m_args.end())
		{
			itr->second.argVec2 = glm::vec2(m_layout.m_width, m_layout.m_height);
		}
		else
		{
			CompArg arg;
			arg.argVec2 = glm::vec2(m_layout.m_width, m_layout.m_height);
			m_meshComp->m_args.insert(std::make_pair(key, arg));
		}
	}

	void VisualAngleView::init()
	{
		if (!m_meshComp->m_initialized)
		{
			Vertex v{};
			v.pos = glm::vec3(-1.f, 1.f, 0.f);
			v.texCoord = glm::vec2(0.f, 1.f);
			m_vertexArray.push_back(v);

			v.pos = glm::vec3(-1.f, -1.f, 0.f);
			v.texCoord = glm::vec2(0.f, 0.f);
			m_vertexArray.push_back(v);

			v.pos = glm::vec3(1.f, 1.f, 0.f);
			v.texCoord = glm::vec2(1.f, 1.f);
			m_vertexArray.push_back(v);

			v.pos = glm::vec3(1.f, -1.f, 0.f);
			v.texCoord = glm::vec2(1.f, 0.f);
			m_vertexArray.push_back(v);

			m_indice.push_back(0);
			m_indice.push_back(1);
			m_indice.push_back(2);
			m_indice.push_back(2);
			m_indice.push_back(1);
			m_indice.push_back(3);

			m_meshComp->initialize(m_vertexArray, m_indice);
		}
	}
}