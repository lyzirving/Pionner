#include "RuntimeLayer.h"
#include "MaterialInspector.h"
#include "ImGuiUtils.h"

#include "GlobalSettings.h"

#include "window/Window.h"
#include "asset/AssetMgr.h"

#include "scene/Scene.h"
#include "scene/Components.h"
#include "scene/3d/Camera.h"
#include "scene/node/CameraNode.h"

#include "event/AppEvent.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/RenderTarget.h"
#include "gfx/rhi/FrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RuntimeLayer"

namespace pio
{
	RuntimeLayer::RuntimeLayer(const LayoutParams& param, const std::string& name) : Layer(param, name)
	{
	}

	void RuntimeLayer::onAttach(Ref<RenderContext>& context)
	{
		auto& window = context->window();
		m_layoutParam.calculate(window->width(), window->height());

		m_sceneParam = LayoutParams(window->width(), window->height(), 0.f, 0.f, 0.6f, 1.f);
		m_hierarchyParam = LayoutParams(window->width(), window->height(), 0.6f, 0.f, 0.75f, 1.f);
		m_inspectorParam = LayoutParams(window->width(), window->height(), 0.75f, 0.f, 1.f, 1.f);

		MaterialInspector::Init(context);
	}

	void RuntimeLayer::onDetach()
	{
		MaterialInspector::Shutdown();
	}

	void RuntimeLayer::onUpdate(Ref<RenderContext>& context, Ref<Scene>& scene)
	{
		if (m_activeScene != scene)
		{
			m_activeScene = scene;
		}
		onDrawMainMenuBar(context, scene);
		onDrawHierarchyView(context, scene);
		onDrawInspectorView(context, scene);
		onDrawSceneView(context, scene);
		m_resetPosition = false;
	}

	bool RuntimeLayer::onEvent(Ref<Event>& event)
	{
		if (!m_activeScene)
		{
			return false;
		}
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowResizeEvent>(PIO_BIND_FN_SELF(RuntimeLayer::onWindowResize, std::placeholders::_1));
		if (event->Handled) { return true; }
		dispatcher.dispatch<KeyPressedEvent>(PIO_BIND_FN_SELF(RuntimeLayer::onKeyPress, std::placeholders::_1));
		if (event->Handled) { return true; }
		dispatcher.dispatch<KeyReleasedEvent>(PIO_BIND_FN_SELF(RuntimeLayer::onKeyRelease, std::placeholders::_1));
		if (event->Handled) { return true; }
		dispatcher.dispatch<MouseMovedEvent>(PIO_BIND_FN_SELF(RuntimeLayer::onMouseMoved, std::placeholders::_1));
		if (event->Handled) { return true; }
		dispatcher.dispatch<MouseButtonPressedEvent>(PIO_BIND_FN_SELF(RuntimeLayer::onMouseBtnPress, std::placeholders::_1));
		if (event->Handled) { return true; }
		dispatcher.dispatch<MouseButtonReleasedEvent>(PIO_BIND_FN_SELF(RuntimeLayer::onMouseBtnRelease, std::placeholders::_1));
		if (event->Handled) { return true; }
		dispatcher.dispatch<MouseScrolledEvent>(PIO_BIND_FN_SELF(RuntimeLayer::onMouseScrolled, std::placeholders::_1));
		return event->Handled;
	}

	void RuntimeLayer::onDrawMainMenuBar(Ref<RenderContext>& context, Ref<Scene>& scene)
	{
		context->submitRC([]()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Project Settings##MenuItem"))
					{
						k_MenuAc = MenuAction_ProjectSettings;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		});

		onDrawMenuAction(context, scene);
	}

	void RuntimeLayer::onDrawMenuAction(Ref<RenderContext>& context, Ref<Scene>& scene)
	{
		switch (k_MenuAc)
		{
			case MenuAction_ProjectSettings:
			{
				onDrawProjectSettings(context, scene);
				break;
			}
			default:
				break;
		}
	}

	void RuntimeLayer::onDrawProjectSettings(Ref<RenderContext>& context, Ref<Scene>& scene)
	{
		context->submitRC([&context]()
		{
			bool bOpen{ true };
			const char* strId = "Project Settings##Popup";
			float aspect = 1.14285f;
			float size = 1.1f * float(std::min(context->window()->width(), context->window()->height()));
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();

			ImGui::OpenPopup(strId);
			ImGui::SetNextWindowSize(ImVec2(size, size / aspect));
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal(strId, &bOpen, ImGuiWindowFlags_AlwaysAutoResize))
			{
				auto avail = ImGui::GetContentRegionAvail();
				ImGui::BeginChild("Project Settings Left", ImVec2(avail.x * 0.3f, avail.y), ImGuiChildFlags_None);
				for (int i = 0; i < ProjectSettingItem_Num; i++)
				{
					if (ImGui::Selectable(k_ProjectSettingItemName[i], k_ProjectSettingSel == i))
						k_ProjectSettingSel = i;
				}
				ImGui::EndChild();

				ImGui::SameLine();
				//Vertical Separator
				auto windowPos = ImGui::GetWindowPos();
				auto windowSize = ImGui::GetWindowSize();
				ImGui::GetWindowDrawList()->AddLine(ImVec2(windowPos.x + avail.x * 0.3f + 1, windowPos.y),
													ImVec2(windowPos.x + avail.x * 0.3f + 1, windowPos.y + windowSize.y),
													ImGui::GetColorU32(ImGuiCol_Separator),
													/*ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f))*/
													2.f);
				ImGui::SameLine();

				avail = ImGui::GetContentRegionAvail();
				ImGui::BeginChild("Project Settings Right", ImVec2(avail.x, avail.y), ImGuiChildFlags_None);

				switch (k_ProjectSettingSel)
				{
					case ProjectSettingItem_Graphics:
					{
						// -------------- LightMap Mode --------------						
						ImGui::AlignTextToFramePadding();
						ImGui::Text("LightMap Mode");
						const char* shadowTechNames[PIO_UINT8(LightTech::Num)] = { "Shadow Map", "CSM" };
						int32_t idx = (int32_t)GlobalSettings::RenderConfig.LightingTech;
						ImGui::AlignTextToFramePadding();
						ImGui::Text(" Shadow Tech ");
						ImGui::SameLine();
						ImGui::Combo("##Shadow Tech", &idx, shadowTechNames, (int32_t)LightTech::Num);
						GlobalSettings::RenderConfig.LightingTech = LightTech(idx);
						if (GlobalSettings::RenderConfig.LightingTech == LightTech::CascadeShadowMap)
						{
							ImGui::AlignTextToFramePadding();
							ImGui::Text(" Indicate CSM");
							ImGui::SameLine();
							ImGui::Checkbox("##Indicate CSM CB", &GlobalSettings::RenderConfig.IndicateCSM);
						}
						// -------------------------------------------
						break;
					}
					default:
						break;
				}

				ImGui::EndChild();

				ImGui::EndPopup();
			}

			if (!bOpen)
			{
				k_MenuAc = MenuAction_Num;
			}
		});
	}

	void RuntimeLayer::onDrawSceneView(Ref<RenderContext>& context, Ref<Scene>& scene)
	{
		auto& param = m_sceneParam;
		bool resetPos = m_resetPosition;
		context->submitRC([resetPos, &context, &scene, &param]()
		{
			if (resetPos)
			{
				auto* mainVp = ImGui::GetMainViewport();
				ImVec2 pos = mainVp->Pos;
				pos.x += param.Rect.Left;
				pos.y += param.Rect.Top;
				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(ImVec2(param.Rect.width(), param.Rect.height()));
			}
			auto camera = AssetMgr::GetRuntimeAsset<Camera>(scene->cameraNode()->camera()->assetHnd());
			const auto& texture = scene->cameraNode()->renderTarget()->frameBuffer()->colorBuffers()[0];
			ImGui::Begin("Scene", 0, ImGuiUtils::k_FlagCommonWindow);
			auto availSize = ImGui::GetContentRegionAvail();
			float aspect0 = availSize.x / availSize.y;
			float aspect1 = float(texture->width()) / float(texture->height());
			glm::vec2 imgSize;
			//Let image always fills the window size
			if (aspect0 < aspect1)
			{
				imgSize.x = aspect1 * availSize.y;
				imgSize.y = availSize.y;
			}
			else
			{
				imgSize.x = availSize.x;
				imgSize.y = availSize.x / aspect1;
			}
			ImGuiUtils::DrawImage(texture->id(), imgSize, glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f), 0.f, 0.f);
			ImGui::End();
		});
	}

	void RuntimeLayer::onDrawHierarchyView(Ref<RenderContext>& context, Ref<Scene>& scene)
	{
		auto& param = m_hierarchyParam;
		auto& selectIdx = m_selectEntIdx;
		bool resetPos = m_resetPosition;
		context->submitRC([resetPos, &context, &scene, &param, &selectIdx]()
		{
			if (resetPos)
			{
				auto* mainVp = ImGui::GetMainViewport();
				ImVec2 pos = mainVp->Pos;
				pos.x += param.Rect.Left;
				pos.y += param.Rect.Top;
				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(ImVec2(param.Rect.width(), param.Rect.height()));
			}

			const auto& nodes = scene->nodes();
			ImGui::Begin("Hierachy", 0, ImGuiUtils::k_FlagCommonWindow);
			ImGuiUtils::ShowHierarchy(nodes, selectIdx);
			ImGui::End();
		});
	}

	void RuntimeLayer::onDrawInspectorView(Ref<RenderContext>& context, Ref<Scene>& scene)
	{
		auto& param = m_inspectorParam;
		uint32_t& selectIdx = m_selectEntIdx;
		bool resetPos = m_resetPosition;
		context->submitRC([resetPos, &context, &scene, &param, &selectIdx]()
		{
			if (resetPos)
			{
				auto* mainVp = ImGui::GetMainViewport();
				ImVec2 pos = mainVp->Pos;
				pos.x += param.Rect.Left;
				pos.y += param.Rect.Top;
				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(ImVec2(param.Rect.width(), param.Rect.height()));
			}
			ImGui::Begin("Inspector", 0, ImGuiUtils::k_FlagCommonWindow);
			auto node = scene->getNode(selectIdx);
			if (node)
			{
				ImGuiUtils::ShowNode(node);
			}
			ImGui::End();
		});
	}

	bool RuntimeLayer::onKeyPress(Ref<KeyPressedEvent>& event)
	{
		//LOGD("key code[%u], repeat[%u]", event->getKeyCode(), event->IsRepeat());
		m_camController.LeftAltPressed = event->getKeyCode() == Key::LeftAlt;
		if (m_camController.RightMouseBtnPressed && m_activeScene)
		{
			auto* comp = m_activeScene->cameraNode()->getComponent<TransformComponent>();
			auto dir = Camera::GetMotionDir(m_activeScene->cameraNode(), event->getKeyCode());
			comp->Position = comp->Position + dir * (float)Time::k_Tick.Delta * 0.01f;
		}
		if (!m_camController.LeftAltPressed) { m_camController.bRotating = false; }
		return false;
	}

	bool RuntimeLayer::onKeyRelease(Ref<KeyReleasedEvent>& event)
	{
		//LOGD("key code[%u]", event->getKeyCode());
		m_camController.LeftAltPressed = false;
		m_camController.bRotating = false;
		return false;
	}

	bool RuntimeLayer::onMouseMoved(Ref<MouseMovedEvent>& event)
	{
		//LOGD("mouse move[%f, %f]", event->getCursorX(), event->getCursorY());
		m_camController.InArea = m_sceneParam.Rect.contain(event->getCursorX(), event->getCursorY());
		if (!m_camController.bRotating && m_camController.InArea && m_camController.LeftAltPressed)
		{
			m_camController.bRotating = true;
		}
		else if (m_camController.bRotating && m_camController.LeftAltPressed)
		{
			//Do nothing, keep current state
		}
		else
		{
			m_camController.bRotating = false;
		}

		if (m_camController.bRotating)
		{
			auto delta = -glm::vec2(event->getCursorX() - m_camController.Cursor.x,
									event->getCursorY() - m_camController.Cursor.y);
			float threshold = 20.f;
			bool valid = std::abs(delta.x) < threshold && std::abs(delta.y) < threshold;
			if (valid && m_activeScene)
			{
				//LOGD("rotating, delta[%f, %f]", delta.x, delta.y);
				auto* comp = m_activeScene->cameraNode()->getComponent<TransformComponent>();
				float ratio = 0.3f;
				comp->Rotation.y += delta.x * ratio;
				comp->Rotation.x += delta.y * ratio;
			}
			m_camController.Cursor.x = event->getCursorX();
			m_camController.Cursor.y = event->getCursorY();
		}
		return m_camController.bRotating;
	}

	bool RuntimeLayer::onMouseBtnPress(Ref<MouseButtonPressedEvent>& event)
	{
		if (m_camController.InArea)
		{
			m_camController.RightMouseBtnPressed = event->getMouseButton() == Mouse::ButtonRight;
		}
		else
		{
			m_camController.RightMouseBtnPressed = false;
		}
		return false;
	}

	bool RuntimeLayer::onMouseBtnRelease(Ref<MouseButtonReleasedEvent>& event)
	{
		m_camController.RightMouseBtnPressed = false;
		return false;
	}

	bool RuntimeLayer::onMouseScrolled(Ref<MouseScrolledEvent>& event)
	{
		if (m_camController.InArea)
		{
			float delta = event->getYOffset() * 0.5f;
			auto* comp = m_activeScene->cameraNode()->getComponent<TransformComponent>();
			auto viewDir = Camera::GetViewDir(m_activeScene->cameraNode());
			comp->Position = comp->Position + viewDir * delta;
			//LOGD("scroll delta[%f], pos[%f, %f, %f]", delta, comp->Position.x, comp->Position.y, comp->Position.z);
			return true;
		}
		return false;
	}

	bool RuntimeLayer::onWindowResize(Ref<WindowResizeEvent>& event)
	{
		m_layoutParam.calculate(event->getWidth(), event->getHeight());
		m_sceneParam.calculate(event->getWidth(), event->getHeight());
		m_hierarchyParam.calculate(event->getWidth(), event->getHeight());
		m_inspectorParam.calculate(event->getWidth(), event->getHeight());
		m_resetPosition = true;
		return false;
	}
}