#include "ScenePanel.h"
#include "ui/imgui/ImGuiUtils.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "scene/Scene.h"
#include "scene/node/CameraNode.h"

#include "component/CameraComponent.h"
#include "component/TransformComponent.h"

#include "event/MouseEvent.h"
#include "event/MouseCodes.h"
#include "event/KeyCodes.h"

#include <imgui.h>
#include <imgui_internal.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ScenePanel"

namespace pio
{
	ScenePanel::ScenePanel(const std::string& name) : ImGuiWindow(name)
	{
	}

	void ScenePanel::OnDraw(const Ref<RenderContext>& context)
	{
		if(!IsSceneBound())
			return;

		auto scene = GetScene();
		auto corlorBuff = scene->GetMainCamera()->GetTarget()->ColorBuffers()[0];

		float aspect0 = Width() / Height();
		float aspect1 = float(corlorBuff->Width()) / float(corlorBuff->Height());
		glm::vec2 imgSize;
		//Let image always fills the window size
		if(aspect0 < aspect1)
		{
			imgSize.x = aspect1 * Height();
			imgSize.y = Height();
		}
		else
		{
			imgSize.x = Width();
			imgSize.y = Width() / aspect1;
		}
		ImGuiUtils::DrawImage(corlorBuff->Id(), imgSize, glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f), 0.f, 0.f);
		OnDrawFpsText();
	}

	bool ScenePanel::OnEvent(const Ref<Event>& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(PIO_BIND_FN_SELF(ScenePanel::OnMouseScrolled, std::placeholders::_1));
		return event->Handled;
	}

	void ScenePanel::OnTick(const Ref<RenderContext>& context)
	{
		if(!IsSceneBound())
			return;

		if(!Contain(Mouse::PosX(), Mouse::PosY()) && !m_bMouseRightDown)
			return;

		if(Mouse::IsRightBtnDown())
		{
			if(Key::AnyPressed())
			{
				auto transComp = GetScene()->GetMainCamera()->GetComponent<TransformComponent>();
				auto camComp = GetScene()->GetMainCamera()->GetComponent<CameraComponent>();
				auto dir = GetCameraHmiMotion(camComp, Key::Code());
				transComp->SetPosition(transComp->GetPosition() + dir * (float)Time::DeltaTimeMs() * 0.01f);
			}

			glm::vec2 diff{ 0.f };
			if(!m_bMouseRightDown)
			{
				m_bMouseRightDown = true;
			}
			else
			{
				diff.x = Mouse::PosX() - m_CursorPos.x;
				diff.y = Mouse::PosY() - m_CursorPos.y;
				diff *= -1.f;
			}
			auto comp = GetScene()->GetMainCamera()->GetComponent<TransformComponent>();
			float ratio = 0.3f;
			glm::vec3 euler = comp->GetRotation().Angle();
			euler.y += diff.x * ratio;
			euler.x += diff.y * ratio;
			comp->SetRotation(euler);

			m_CursorPos.x = Mouse::PosX();
			m_CursorPos.y = Mouse::PosY();
		}
		else
		{
			m_bMouseRightDown = false;
		}
	}

	bool ScenePanel::OnMouseScrolled(const Ref<MouseScrolledEvent>& event)
	{
		if(!IsSceneBound())
			return false;

		if(!Contain(Mouse::PosX(), Mouse::PosY()))
			return false;

		float delta = event->OffsetY() * 0.5f;
		auto camNode = GetScene()->GetMainCamera();		
		auto transComp = camNode->GetComponent<TransformComponent>();
		auto camComp = camNode->GetComponent<CameraComponent>();
		transComp->SetPosition(transComp->GetPosition() + camComp->GetViewDir() * delta);
		return true;
	}

	void ScenePanel::OnDrawFpsText()
	{
		std::string content("fps: ");
		content.append(std::to_string(Time::Fps()));
		ImVec2 position = ImVec2(Left() + 5, Top() + 5);		
		ImGui::GetForegroundDrawList()->AddText(position, ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f)), content.c_str());
	}

	glm::vec3 ScenePanel::GetCameraHmiMotion(const Ref<CameraComponent>& comp, uint16_t key)
	{
		glm::vec3 dir(0.f);
		switch(key)
		{
			case Key::A:
			{
				dir = -comp->GetCameraRight();
				break;
			}
			case Key::D:
			{
				dir = comp->GetCameraRight();
				break;
			}
			case Key::W:
			{
				dir = -comp->GetCameraFront();
				break;
			}
			case Key::S:
			{
				dir = comp->GetCameraFront();
				break;
			}
			case Key::Q:
			{
				dir = -comp->GetCameraUp();
				break;
			}
			case Key::E:
			{
				dir = comp->GetCameraUp();
				break;
			}
			default:
				break;
		}
		return dir;
	}
}