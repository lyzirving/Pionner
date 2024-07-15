#include "UiPanel.h"

#include "ui/ImGuiUtils.h"
#include <imgui.h>

#include "scene/Entity.h"
#include "scene/Components.h"

namespace pio
{
	void UiPanel::DrawNamePanel(const char* nLabel, const std::string& name, const char* vLabel, bool& visible, uint32_t rowWidth)
	{
		float cbWidth = 0.2f * rowWidth;
		float textWidth = 0.8f * rowWidth;
		
		ImGui::PushItemWidth(cbWidth);
		ImGui::Checkbox(vLabel, &visible);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushItemWidth(textWidth);
		ImGui::InputText(nLabel, const_cast<char*>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
	}

	void UiPanel::DrawTransformPanel(Transform& transform)
	{
		if (ImGui::CollapsingHeader("Transform", ImGuiUtils::Flag_Collapse_Header))
		{
			ImGui::DragFloat3("Position##Transform", glm::value_ptr(transform.Position), 0.05f, -100.f, 100.f, "%.1f");
			glm::vec3 angle = transform.Euler.angle();
			ImGui::DragFloat3("Rotation##Transform", &angle.x, 0.1f, -360.f, 360.f, "%.1f");
			transform.Euler = angle;
			ImGui::DragFloat3("Scale##Transform", glm::value_ptr(transform.Scale), 0.1f, 0.f, 10.f, "%.1f");
		}
	}

	void UiPanel::DrawTransformPanel(Ref<Entity>& entity)
	{
		if (!entity || !entity->hasComponent<TransformComponent>())
			return;

		auto& comp = entity->getComponent<TransformComponent>();
		DrawTransformPanel(comp.Transform);
	}
}