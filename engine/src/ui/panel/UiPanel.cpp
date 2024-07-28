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
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::DragFloat3("##Transform_Position", glm::value_ptr(position), 0.05f, -100.f, 100.f, "%.1f");
			if (transform.Position != position)
			{
				transform.Position = position;
				change.set(DataAttrBits_Pos);
			}

			glm::vec3 angle = transform.Euler.angle();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::DragFloat3("##Transform_Rotation", &angle.x, 0.1f, -360.f, 360.f, "%.1f");
			if (transform.Euler != angle)
			{
				transform.Euler = angle;
				change.set(DataAttrBits_Rot);
			}

			ImGui::AlignTextToFramePadding();
			//Enough text padding for text alignment
			ImGui::Text("Scale   ");
			ImGui::SameLine();
			ImGui::DragFloat3("##Transform_Scale", glm::value_ptr(transform.Scale), 0.1f, 0.f, 10.f, "%.1f");
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
			float textWidth = 0.8f * itemSize.x;
			std::string_view className = entity->getClassName();

			ImGui::PushItemWidth(labelWidth);
			ImGui::LabelText("##light_type_label", "Type");
			ImGui::PopItemWidth();

			ImGui::SameLine();

			ImGui::PushItemWidth(textWidth);
			ImGui::InputText("##light_type", const_cast<char*>(className.data()), className.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
		}
	}

	void UiPanel::DrawCameraPanel(CameraComponent& comp)
	{
		if (ImGui::CollapsingHeader("Camera", ImGuiUtils::Flag_Collapse_Header))
		{
			auto& camera = comp.Camera;

			float fov = camera.fov();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Fov       ");
			ImGui::SameLine();
			ImGui::DragFloat("##FOV", &fov, 1.f, 0.1, 179.f, "%.1f");
			camera.setFov(fov);

			int prjType{ camera.prjType() };
			const char* items[2]{ "Perspective", "Orthographic" };
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Projection");
			ImGui::SameLine();
			ImGui::Combo("##Prj_type", &prjType, items, ProjectionType_Num);
			camera.setPrjType(ProjectionType(prjType));

			float near = camera.frustNear();
			float far = camera.frustFar();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Near      ");
			ImGui::SameLine();
			ImGui::DragFloat("##Near", &near, 1.f, 0.1, 100.f, "%.1f");
			ImGui::AlignTextToFramePadding(); 
			ImGui::Text("Far       ");
			ImGui::SameLine();
			ImGui::DragFloat("##Far", &far, 1.f, 0.1, 1000.f, "%.1f");
			camera.setNear(near);
			camera.setFar(far);
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