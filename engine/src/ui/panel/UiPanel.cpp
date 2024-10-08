#include "UiPanel.h"

#include "ui/ImGuiUtils.h"
#include <imgui.h>

#include "asset/AssetsManager.h"

#include "gfx/renderer/Renderer.h"

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

		bool bVisible = true;
		DrawNamePanel("##light_name", entity->getName(), "##light_visibility", bVisible);
		DrawTransformPanel(entity);

		if (ImGui::CollapsingHeader("Light", ImGuiUtils::Flag_Collapse_Header))
		{
			std::string_view className = entity->getClassName();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Type       ");
			ImGui::SameLine();
			ImGui::InputText("##light_type", const_cast<char*>(className.data()), className.size(), ImGuiInputTextFlags_ReadOnly);

			if (entity->hasComponent<DirectionalLightComponent>())
			{
				DrawDirectionalLightPanel(entity);
			}
			else if (entity->hasComponent<PointLightComponent>())
			{
				DrawPointLightPanel(entity);
			}
		}
	}

	void UiPanel::DrawCameraPanel(Ref<Entity>& entity)
	{
		if (!entity || !entity->hasComponent<CameraComponent>())
			return;

		auto& comp = entity->getComponent<CameraComponent>();
		auto camera = AssetsManager::GetRuntimeAsset<Camera>(comp.Handle);
		bool visible{ true };// always be visible

		UiPanel::DrawNamePanel("##camera_name", entity->getName(), "##camera_visibility", visible);

		auto attr = UiPanel::DrawTransformPanel(camera->transform());
		if (attr.test(DataAttrBits_Pos))
			camera->attrChange(CameraAttrBits_Pos);
		if (attr.test(DataAttrBits_Rot))
			camera->attrChange(CameraAttrBits_Rot);

		if (ImGui::CollapsingHeader("Camera", ImGuiUtils::Flag_Collapse_Header))
		{
			float itemWidth = ImGui::GetItemRectSize().x;

			auto camera = AssetsManager::GetRuntimeAsset<Camera>(comp.Handle);

			bool bMain = comp.Primary;
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Main      ");
			ImGui::SameLine();
			ImGui::Checkbox("##Main", &bMain);

			const int flagNum = 2;
			int clearFlag{ camera->clearFlag() };
			const char* flagItems[flagNum]{ "Skybox", "Color" };
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Clear Flag");
			ImGui::SameLine();
			ImGui::Combo("##Clear_Flag", &clearFlag, flagItems, flagNum);
			camera->setClearFlag(CameraClearFlags(clearFlag));

			switch (clearFlag)
			{
			case CameraClearFlag_Skybox:
			{
				Ref<Skybox> sk = camera->skybox();
				float intensity = sk->getIntensity();
				const auto& name = sk->getName();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Name      ");
				ImGui::SameLine();
				ImGui::Text("%s", name.c_str());

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Intensity ");
				ImGui::SameLine();
				ImGui::SliderFloat("##Intensity", &intensity, 0.0001f, 0.3f, "%.4f");
				sk->setIntensity(intensity);
				break;
			}
			case CameraClearFlag_Color:
			{
				auto& color = Renderer::GetConfig().ClearColor;

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Color     ");
				ImGui::SameLine();
				ImGui::ColorEdit4("##Bg Color", &color.r);
				break;
			}
			default:
				break;
			}

			int prjType{ camera->prjType() };
			const char* items[2]{ "Perspective", "Orthographic" };
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Projection");
			ImGui::SameLine();
			ImGui::Combo("##Prj_type", &prjType, items, ProjectionType_Num);
			camera->setPrjType(ProjectionType(prjType));

			if (prjType == ProjectionType_Perspective)
			{
				float fov = camera->fov();
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Fov       ");
				ImGui::SameLine();
				ImGui::DragFloat("##FOV", &fov, 1.f, 0.1, 179.f, "%.1f");
				camera->setFov(fov);
			}
			else
			{
				float size = camera->size();
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Size      ");
				ImGui::SameLine();
				ImGui::DragFloat("##Size", &size, 0.1f, 0.1, 100.f, "%.1f");
				camera->setSize(size);
			}

			float near = camera->frustNear();
			float far = camera->frustFar();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Near      ");
			ImGui::SameLine();
			ImGui::DragFloat("##Near", &near, 1.f, 0.1, 100.f, "%.1f");
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Far       ");
			ImGui::SameLine();
			ImGui::DragFloat("##Far", &far, 1.f, 0.1, 1000.f, "%.1f");
			camera->setNear(near);
			camera->setFar(far);

			auto& vp = camera->viewport();
			float x{ vp.offsetX() }, y{ vp.offsetY() };
			float w{ vp.ratioW() }, h{ vp.ratioH() };
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Viewport Rect");

			ImGui::AlignTextToFramePadding();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth * 0.4f);
			ImGui::DragFloat("##Viewport_X", &x, 0.01f, 0.f, 1.f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth * 0.4f);
			ImGui::DragFloat("##Viewport_Y", &y, 0.01f, 0.f, 1.f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("W");
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth * 0.4f);
			ImGui::DragFloat("##Viewport_W", &w, 0.01f, 0.f, 1.f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text("H");
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth * 0.4f);
			ImGui::DragFloat("##Viewport_H", &h, 0.01f, 0.f, 1.f, "%.2f");
			ImGui::PopItemWidth();

			vp.setOffsetX(x); vp.setOffsetY(y);
			vp.setRatioW(w);  vp.setRatioH(h);
		}
	}

	void UiPanel::DrawDirectionalLightPanel(Ref<Entity>& entity)
	{
		auto& comp = entity->getComponent<DirectionalLightComponent>();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Color      ");
		ImGui::SameLine();
		ImGui::ColorEdit3("##Light_Radiance", &comp.Radiance.r);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Mode       ");
		ImGui::SameLine();
		ImGui::InputText("##Light_Mode", "Real Time", 10, ImGuiInputTextFlags_ReadOnly);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Intensity  ");
		ImGui::SameLine();
		ImGui::DragFloat("##Light_Intensity", &comp.Intensity, 0.02f, 0.1f, 10.f, "%.2f", 0);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Shadow Type");
		ImGui::SameLine();
		ImGui::Combo("##Shadow_Type", &comp.SdMode, ShadowModeNames, ShadowMode_Num);
		comp.CastShadow = comp.SdMode != ShadowMode_None;
		if (comp.CastShadow)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  Realtime Shadows");

			ImGui::AlignTextToFramePadding();
			ImGui::Text("    Strength   ");
			ImGui::SameLine();
			auto remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			ImGui::DragFloat("##Sd_Intensity", &comp.SdIntensity, 0.02f, 0.f, 1.f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("    Bias       ");
			ImGui::SameLine();
			remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			ImGui::DragFloat("##Sd_Bias", &comp.Bias, 0.01f, 0.f, 2.f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("    Normal Bias");
			ImGui::SameLine();
			remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			ImGui::DragFloat("##Normal_Bias", &comp.NormalBias, 0.01f, 0.f, 3.f, "%.2f");
			ImGui::PopItemWidth();
		}
	}

	void UiPanel::DrawPointLightPanel(Ref<Entity> &entity)
	{
		auto &comp = entity->getComponent<PointLightComponent>();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Range      ");
		ImGui::SameLine();
		ImGui::DragFloat("##Light_Range", &comp.Radius, 0.1f, 0.1f, 500.f, "%.1f");

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Color      ");
		ImGui::SameLine();
		ImGui::ColorEdit3("##Light_Radiance", &comp.Radiance.r);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Mode       ");
		ImGui::SameLine();
		ImGui::InputText("##Light_Mode", "Real Time", 10, ImGuiInputTextFlags_ReadOnly);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Intensity  ");
		ImGui::SameLine();
		ImGui::DragFloat("##Light_Intensity", &comp.Intensity, 0.02f, 0.1f, 100.f, "%.2f");

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Falloff    ");
		ImGui::SameLine();
		ImGui::DragFloat("##Light_Falloff", &comp.Falloff, 0.02f, 0.1f, 100.f, "%.2f");

		int sdMode = ShadowMode_None;
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Shadow Type");
		ImGui::SameLine();
		ImGui::Combo("##Shadow_Type", &sdMode, ShadowModeNames, ShadowMode_Num);
	}
}