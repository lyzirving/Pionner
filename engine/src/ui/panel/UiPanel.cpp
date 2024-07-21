#include "UiPanel.h"

#include "ui/ImGuiUtils.h"
#include <imgui.h>

#include "scene/Entity.h"
#include "scene/Components.h"

namespace pio
{
	void UiPanel::DrawNamePanel(const char* nLabel, const std::string& name, const char* vLabel, bool& visible, uint32_t rowWidth)
	{
		if (rowWidth == 0)
		{
			rowWidth = ImGui::GetItemRectSize().x;
		}
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

	DataAttrs UiPanel::DrawTransformPanel(Transform& transform)
	{
		DataAttrs change;
		if (ImGui::CollapsingHeader("Transform", ImGuiUtils::Flag_Collapse_Header))
		{
			glm::vec3 position = transform.Position.ccs();
			ImGui::DragFloat3("Position##Transform", glm::value_ptr(position), 0.05f, -100.f, 100.f, "%.1f");
			if (transform.Position != position)
			{
				transform.Position = position;
				change.set(DataAttrBits_Pos);
			}

			glm::vec3 angle = transform.Euler.angle();
			ImGui::DragFloat3("Rotation##Transform", &angle.x, 0.1f, -360.f, 360.f, "%.1f");
			if (transform.Euler != angle)
			{
				transform.Euler = angle;
				change.set(DataAttrBits_Rot);
			}	

			ImGui::DragFloat3("Scale##Transform", glm::value_ptr(transform.Scale), 0.1f, 0.f, 10.f, "%.1f");
		}
		return change;
	}

	DataAttrs UiPanel::DrawTransformPanel(Ref<Entity>& entity)
	{
		if (!entity || !entity->hasComponent<TransformComponent>())
			return DataAttrs();

		auto& comp = entity->getComponent<TransformComponent>();
		return DrawTransformPanel(comp.Transform);
	}

	void UiPanel::DrawLightPanel(Ref<Entity>& entity)
	{
		if (!entity)
			return;

		DrawTransformPanel(entity);

		if (ImGui::CollapsingHeader("Light", ImGuiUtils::Flag_Collapse_Header))
		{
			auto itemSize = ImGui::GetItemRectSize();
			float labelWidth = 0.2f * itemSize.x;
			float textWidth  = 0.8f * itemSize.x;
			std::string_view className = entity->getClassName();

			ImGui::PushItemWidth(labelWidth);
			ImGui::LabelText("##light_type_label", "Type");
			ImGui::PopItemWidth();

			ImGui::SameLine();	

			ImGui::PushItemWidth(textWidth);
			ImGui::InputText("##light_type", const_cast<char *>(className.data()), className.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
		}
	}

	void UiPanel::DrawDirectionalLightPanel(Ref<Entity>& entity)
	{
		if (ImGui::CollapsingHeader("Light", ImGuiUtils::Flag_Collapse_Header))
		{
		}
	}

	void UiPanel::DrawPointLightPanel(Ref<Entity>& entity)
	{
	}
}