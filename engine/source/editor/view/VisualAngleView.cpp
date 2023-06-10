#include "VisualAngleView.h"

#include "function/ui/WindowUI.h"

#include "function/render/RenderDef.h"
#include "function/render/rhi/Rhi.h"

#include "function/render/scene/SceneMgr.h"
#include "function/render/scene/Frustum.h"
#include "function/render/scene/Camera.h"

#include "function/render/geo/CoordinateAixs.h"
#include "function/render/geo/Sphere.h"

#include "function/event/EventMgr.h"

#include "function/render/resource/ResourceDef.h"

#include "core/log/LogSystem.h"
#include "core/math/MathLib.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VisualAngleView"

namespace Pionner
{
	const float VisualAngleView::CAM_SCROLL_SCALE = 5.f;

	VisualAngleView::VisualAngleView() : WindowView(ORDER_VIEW_1)
		, m_coordinateAxis(new CoordinateAxis("coordinate axis"))
		, m_sphere(new Sphere(50, 50, "background sphere"))
		, m_renderport()
		, m_lastCursorPos(INVALID_CURSOR_POS, INVALID_CURSOR_POS)
	{
		m_uid = UID_VISUAL_ANGLE;
	}

	VisualAngleView::~VisualAngleView()
	{
		m_coordinateAxis.reset();
		m_sphere.reset();
	}

	void VisualAngleView::draw(RenderParam &param)
	{
		m_coordinateAxis->initialize(param);
		m_sphere->initialize(param);

		auto evtMgr = param.rhi->getWindowSystem()->getEvtMgr();
		bool showBackground = m_layout.contains(evtMgr->getCursorPosX(), evtMgr->getCursorPosY());

		auto rhi = param.rhi;
		auto sceneMgr = param.sceneMgr;
		auto frustum = sceneMgr->m_frustum;
		auto camera = sceneMgr->m_camera;
		auto cmd = rhi->getDrawCmd();

		rhi->restoreViewportState();
		frustum->restoreState();
		camera->restoreState();

		rhi->viewportSub(m_renderport.m_left, m_renderport.m_top, m_renderport.m_width, m_renderport.m_height);

		frustum->setAspect(float(m_renderport.m_width) / float(m_renderport.m_height));

		if (showBackground)
		{
			glm::vec3 backCamPos{ 0.f, 0.f, 1.f };
			camera->setPosition(backCamPos);

			m_sphere->translate(0.f, 0.f, -1.f);
			m_sphere->draw(param);

			camera->popState();
			camera->restoreState();
		}

		glm::vec3 stdPos = camera->getCamPos();
		stdPos = glm::normalize(stdPos);
		camera->setPosition(stdPos);
		frustum->setFov(frustum->fov() + 10.f);

		m_coordinateAxis->draw(param);

		camera->popState();
		frustum->popState();
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
	}

	bool VisualAngleView::processEvent(RenderParam &param, const Event &evt)
	{
		switch (evt.m_type)
		{
			case EVENT_TYPE_PRESSED_MOVING:
			{
				if (!MathLib::equalF(m_lastCursorPos.x, INVALID_CURSOR_POS) && !MathLib::equalF(m_lastCursorPos.y, INVALID_CURSOR_POS))
				{
					glm::vec2 delta{ evt.m_posX, evt.m_posY };
					delta -= m_lastCursorPos;
					auto  camera = param.sceneMgr->m_camera;
					camera->dealScrollPosition(delta.x, delta.y);
				}
				m_lastCursorPos.x = evt.m_posX;
				m_lastCursorPos.y = evt.m_posY;
				return true;
			}
			case EVENT_TYPE_PRESSED_MOVING_FINISH:
			{
				m_lastCursorPos.x = INVALID_CURSOR_POS;
				m_lastCursorPos.y = INVALID_CURSOR_POS;
				return true;
			}
			case EVENT_TYPE_SCROLLING:
			{
				auto  camera = param.sceneMgr->m_camera;
				camera->dealScrollPosition(evt.m_scrollDeltaX * CAM_SCROLL_SCALE, evt.m_scrollDeltaY * CAM_SCROLL_SCALE);
				return true;
			}
			default:
				break;
		}
		return false;
	}

	void VisualAngleView::resetMotion()
	{
		m_lastCursorPos.x = INVALID_CURSOR_POS;
		m_lastCursorPos.y = INVALID_CURSOR_POS;
	}

}