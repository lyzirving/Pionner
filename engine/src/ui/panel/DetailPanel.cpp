#include "DetailPanel.h"
#include "ui/imgui/ImGuiUtils.h"

#include "gfx/rhi/Texture.h"
#include "gfx/renderer/RenderContext.h"

#include "scene/Scene.h"
#include "scene/node/CameraNode.h"
#include "scene/node/StaticMeshNode.h"
#include "scene/node/SkinnedMeshNode.h"
#include "scene/node/DirectionalLitNode.h"
#include "scene/node/PointLitNode.h"
#include "scene/node/MaterialBall.h"

#include "component/TransformComponent.h"
#include "component/StaticMeshComponent.h"
#include "component/SkeletalMeshComponent.h"

#include "asset/mesh/StaticMesh.h"
#include "asset/mesh/SkinnedMesh.h"
#include "asset/mesh/AnimationClip.h"

#include "asset/image/Image.h"
#include "asset/material/Material.h"
#include "asset/material/StandardMaterial.h"
#include "asset/material/TexturedMaterial.h"

#include "animation/AnimationState.h"

#include <imgui.h>
#include <imgui_internal.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DetailPanel"

namespace pio
{
	DetailPanel::DetailPanel(const std::string& name) : ImGuiWindow(name)
	{
	}

	void DetailPanel::OnAttach(const Ref<Scene>& scene)
	{
		ImGuiWindow::OnAttach(scene);
		m_MaterialBall = scene->CreateNode<MaterialBall>();		
		m_MaterialBall->AttachScene(scene);
		AddSlotCallback(EVENT_ID_ON_NODE_SELECT, EventCallback(this, (SLOTFUNCTION)&DetailPanel::OnNodeSelect));
	}

	void DetailPanel::OnDetach()
	{		
		ImGuiWindow::OnDetach();
		m_MaterialBall->DetachScene();
		RemoveSlotCallback(EVENT_ID_ON_NODE_SELECT, EventCallback(this, (SLOTFUNCTION)&DetailPanel::OnNodeSelect));
	}

	void DetailPanel::OnDraw(const Ref<RenderContext>& context)
	{
		if(!IsSceneBound())
		{
			LOGE("scene is not bound");
			return;
		}
		if(m_Select == InvalidId)
			return;

		auto node = GetScene()->FindNode(m_Select);
		if(node->Is<CameraNode>())
		{
			ShowCameraDetail(node->As<CameraNode>());
		}
		else if(node->Is<DirectionalLitNode>())
		{
			ShowDirectionalLitDetail(node->As<DirectionalLitNode>());
		}
		else if(node->Is<PointLitNode>())
		{
			ShowPointLitDetail(node->As<PointLitNode>());
		}
		else if(node->Is<StaticMeshNode>())
		{
			ShowStaticMeshDetail(node->As<StaticMeshNode>());
		}
		else if(node->Is<SkinnedMeshNode>())
		{
			ShowSkinnedMeshDetail(node->As<SkinnedMeshNode>());
		}
	}

	void DetailPanel::OnNodeSelect(const Ref<Event>& event)
	{
		m_Select = event->As<NodeSelectEvent>()->Select;
	}

	void DetailPanel::ShowCameraDetail(CameraNode* node)
	{
		DrawTransformCompDetail(node->GetComponent<TransformComponent>());

		if(ImGui::CollapsingHeader("Camera", ImGuiUtils::k_FlagOpenCollapseHeader))
		{
			float near = node->GetNear();
			float far = node->GetFar();
			int32_t type = node->GetProjectionType();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Near      ");
			ImGui::SameLine();
			ImGui::DragFloat("##Near", &near, 1.f, 0.1, 100.f, "%.1f");

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Far       ");
			ImGui::SameLine();
			ImGui::DragFloat("##Far", &far, 1.f, 0.1, 1000.f, "%.1f");

			const char* items[2]{ "Perspective", "Orthographic" };
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Projection");
			ImGui::SameLine();
			ImGui::Combo("##Prj_type", &type, items, ProjectionType_Num);

			if(type == ProjectionType_Perspective)
			{
				float fov = node->GetFov();
				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Fov       ");
				ImGui::SameLine();
				ImGui::DragFloat("##FOV", &fov, 1.f, 0.1, 179.f, "%.1f");
				node->SetFov(fov);
			}
			else
			{
				float size = node->GetOrthoSize();
				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Size      ");
				ImGui::SameLine();
				ImGui::DragFloat("##Size", &size, 0.1f, 0.1, 100.f, "%.1f");
				node->SetOrthoSize(size);
			}
			node->SetNear(near);
			node->SetFar(far);
			node->SetProjectionType(ProjectionType(type));
		}
	}

	void DetailPanel::ShowStaticMeshDetail(StaticMeshNode* node)
	{
		DrawTransformCompDetail(node->GetComponent<TransformComponent>());
		DrawStaticMeshCompDetail(node->GetComponent<StaticMeshComponent>());
	}

	void DetailPanel::ShowSkinnedMeshDetail(SkinnedMeshNode* node)
	{
		DrawTransformCompDetail(node->GetComponent<TransformComponent>());
		DrawSkeletalMeshCompDetail(node->GetComponent<SkeletalMeshComponent>());
	}

	void DetailPanel::ShowDirectionalLitDetail(DirectionalLitNode* node)
	{
		DrawTransformCompDetail(node->GetComponent<TransformComponent>());

		if(ImGui::CollapsingHeader("DirectionalLight", ImGuiUtils::k_FlagOpenCollapseHeader))
		{
			auto color = node->GetColor();
			auto intensity = node->GetIntensity();
			int32_t sdMode = node->GetShadowMode();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Color      ");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Light_Color", &color.x);

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Intensity  ");
			ImGui::SameLine();
			ImGui::DragFloat("##Light_Intensity", &intensity, 0.02f, 0.1f, 50.f, "%.2f", 0);

			const char* shadowModeNames[ShadowMode_Num] = { "No Shadows", "Hard", "Soft", "Soft_2X", "Soft_4X" };
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Shadow Type");
			ImGui::SameLine();
			ImGui::Combo("##Shadow_Type", &sdMode, shadowModeNames, ShadowMode_Num);

			node->SetColor(color);
			node->SetIntensity(intensity);
			node->SetShadowMode(ShadowMode(sdMode));

			if(node->IsCastShadow())
			{
				auto sdIntensity = node->GetShadowIntensity();
				auto bias = node->GetShadowBias();

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Realtime Shadows");

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Strength   ");
				ImGui::SameLine();
				auto remain = ImGui::GetContentRegionAvail();
				ImGui::PushItemWidth(remain.x);
				ImGui::DragFloat("##Sd_Intensity", &sdIntensity, 0.02f, 0.f, 1.f, "%.2f");
				ImGui::PopItemWidth();

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Bias       ");
				ImGui::SameLine();
				remain = ImGui::GetContentRegionAvail();
				ImGui::PushItemWidth(remain.x);
				ImGui::DragFloat("##Sd_Bias", &bias, 0.00001f, 0.f, PIO_MAX_DIR_LIT_SHADOW_BIAS, "%.5f");
				ImGui::PopItemWidth();

				node->SetShadowIntensity(sdIntensity);
				node->SetShadowBias(bias);
			}
		}
	}

	void DetailPanel::ShowPointLitDetail(PointLitNode* node)
	{
		DrawTransformCompDetail(node->GetComponent<TransformComponent>());

		if(ImGui::CollapsingHeader("PointLight", ImGuiUtils::k_FlagOpenCollapseHeader))
		{
			auto color = node->GetColor();
			auto intensity = node->GetIntensity();
			auto radius = node->GetRadius();
			auto falloff = node->GetFalloff();
			int32_t shadowMode = node->GetShadowMode();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Index      ");
			ImGui::SameLine();
			auto remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			std::string content = std::to_string(node->GetIndex());
			ImGui::InputText("##PointLitIndex", const_cast<char*>(content.c_str()), content.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Color      ");
			ImGui::SameLine();
			ImGui::ColorEdit3("##PointLitColor", glm::value_ptr(color));

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Intensity  ");
			ImGui::SameLine();
			ImGui::DragFloat("##PointLitIntensity", &intensity, 0.02f, 0.1f, 50.f, "%.2f", 0);

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Radius     ");
			ImGui::SameLine();
			ImGui::DragFloat("##PointLitRadius", &radius, 0.02f, 0.1f, 50.f, "%.2f", 0);

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Falloff    ");
			ImGui::SameLine();
			ImGui::DragFloat("##PointLitFalloff", &falloff, 0.02f, 0.01f, 1.f, "%.2f", 0);

			const char* shadowModeNames[3] = { "No Shadows", "Hard", "Soft" };
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Shadow Type");
			ImGui::SameLine();
			ImGui::Combo("##Shadow_Type", &shadowMode, shadowModeNames, 3);

			node->SetColor(color);
			node->SetIntensity(intensity);
			node->SetRadius(radius);
			node->SetFalloff(falloff);
			node->SetShadowMode(ShadowMode(shadowMode));

			if(node->IsCastShadow())
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Realtime Shadows");

				auto shadowIntensity = node->GetShadowIntensity();
				auto shadowBias = node->GetShadowBias();

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Strength   ");
				ImGui::SameLine();
				auto remain = ImGui::GetContentRegionAvail();
				ImGui::PushItemWidth(remain.x);
				ImGui::DragFloat("##PointLitShadowIntensity", &shadowIntensity, 0.02f, 0.f, 1.f, "%.2f");
				ImGui::PopItemWidth();

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Bias       ");
				ImGui::SameLine();
				remain = ImGui::GetContentRegionAvail();
				ImGui::PushItemWidth(remain.x);
				ImGui::DragFloat("##PointLitShadowBias", &shadowBias, 0.00001f, 0.f, 1.f, "%.5f");
				ImGui::PopItemWidth();

				node->SetShadowIntensity(shadowIntensity);
				node->SetShadowBias(shadowBias);
			}
		}
	}

	void DetailPanel::DrawTransformCompDetail(const Ref<TransformComponent>& comp)
	{
		if(!comp)
			return;

		if(ImGui::CollapsingHeader("Transform", ImGuiUtils::k_FlagOpenCollapseHeader))
		{
			auto position = comp->GetPosition();
			auto rotation = comp->GetRotation().Angle();
			auto scale = comp->GetScale();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Position");
			ImGui::SameLine();
			ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.05f, -100.f, 100.f, "%.1f");

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Rotation");
			ImGui::SameLine();
			ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 0.1f, -361.f, 361.f, "%.1f");
			if(rotation.x > 360.f || rotation.x < -360.f)
				rotation.x = std::fmod(rotation.x, 360.f);

			if(rotation.y > 360.f || rotation.y < -360.f)
				rotation.y = std::fmod(rotation.y, 360.f);

			if(rotation.z > 360.f || rotation.z < -360.f)
				rotation.z = std::fmod(rotation.z, 360.f);

			ImGui::AlignTextToFramePadding();
			//Enough text padding for text alignment
			ImGui::Text(" Scale   ");
			ImGui::SameLine();
			ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f, 0.f, 10.f, "%.1f");

			comp->SetPosition(position);
			comp->SetRotation(rotation);
			comp->SetScale(scale);
		}
	}

	void DetailPanel::DrawStaticMeshCompDetail(const Ref<StaticMeshComponent>& comp)
	{
		DrawStaticMeshDetail(comp->GetMesh());
		DrawMaterialDetail(comp->GetMaterialElems());
	}

	void DetailPanel::DrawStaticMeshDetail(const Ref<StaticMesh>& mesh)
	{
		if(ImGui::CollapsingHeader("Mesh", ImGuiUtils::k_FlagOpenCollapseHeader))
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  Static Mesh Asset");
			ImGui::SameLine();
			auto remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			ImGui::InputText("##Static Mesh Asset Name", const_cast<char*>(mesh->Name().c_str()), mesh->Name().size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
		}
	}

	void DetailPanel::DrawSkeletalMeshCompDetail(const Ref<SkeletalMeshComponent>& comp)
	{
		DrawSkinnedMeshDetail(comp->GetMesh());
		DrawSkeletalAnimationDetail(comp);
		DrawMaterialDetail(comp->GetMaterialElems());
	}

	void DetailPanel::DrawSkinnedMeshDetail(const Ref<SkinnedMesh>& mesh)
	{
		if(ImGui::CollapsingHeader("Mesh", ImGuiUtils::k_FlagOpenCollapseHeader))
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  Skinned Mesh Asset");
			ImGui::SameLine();
			auto remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			ImGui::InputText("##Static Mesh Asset Name", const_cast<char*>(mesh->Name().c_str()), mesh->Name().size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
		}
	}

	void DetailPanel::DrawSkeletalAnimationDetail(const Ref<SkeletalMeshComponent>& comp)
	{
		if(ImGui::CollapsingHeader("Skeletal", ImGuiUtils::k_FlagOpenCollapseHeader))
		{
			const auto& clips = comp->GetMesh()->GetAnimationClips();
			if(clips.empty())
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text(" None Animation");
				return;
			}			
			Ref<AnimationClip> curAnim;
			int32_t selected = clips.size();
			if(comp->IsAnimSet())
			{
				curAnim = comp->GetAnimClip();
				for(size_t i = 0; i < clips.size(); i++)
				{
					if(curAnim == clips[i])
					{
						selected = i;
						break;
					}
				}
			}
			int32_t pending = selected;
			char** names = (char**)malloc(sizeof(char*) * (clips.size() + 2));
			for(size_t i = 0; i < clips.size(); ++i)
			{
				names[i] = strdup(clips[i]->Name().c_str());
			}
			names[clips.size()] = "None Animation";
			// set the last entry to null to signify the end of the list
			names[clips.size() + 1] = '\0'; 
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Animations");
			ImGui::SameLine();
			ImGui::Combo("##AnimationClipName", &pending, names, clips.size() + 1);

			for(size_t i = 0; i < clips.size(); ++i)
			{
				// free memory for each c-style string
				free(names[i]);
			}
			free(names);
			
			if(pending != selected)
			{
				if(pending < clips.size())
				{
					comp->SetAnimationClip(clips[pending]);
					comp->SetLocalTimePos(0.f);
					if(!comp->IsStart())
					{
						comp->Start();
					}
				}
				else
				{
					comp->RemoveAnimationClip();
				}
			}

			if(comp->IsAnimSet())
			{
				bool isStart = comp->IsStart();
				bool isLoop = comp->IsLoop();

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Start");
				ImGui::SameLine();
				ImGui::Checkbox("##AnimStart", &isStart);

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Loop ");
				ImGui::SameLine();
				ImGui::Checkbox("##AnimLoop", &isLoop);

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Duration");
				ImGui::SameLine();
				auto remain = ImGui::GetContentRegionAvail();
				ImGui::PushItemWidth(remain.x);
				std::stringstream ss;
				ss.setf(std::ios::fixed);
				ss.precision(4);
				ss << comp->GetDuration() << "s";
				std::string duration = ss.str();
				ImGui::InputText("##AnimClip Duration", const_cast<char*>(duration.c_str()), duration.size(), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Local Time");
				ImGui::SameLine();
				// Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
				// or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
				auto color = ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_FrameBgHovered];
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
				ImGui::ProgressBar(comp->GetLocalTimePos(), ImVec2(-FLT_MIN, 0.0f));
				ImGui::PopStyleColor();

				if(isStart) { comp->Start(); } else { comp->Stop(); }
				comp->SetLoop(isLoop);
			}			
		}
	}

	void DetailPanel::DrawMaterialDetail(const std::vector<Ref<Material>>& materials)
	{
		if(ImGui::CollapsingHeader("Material", ImGuiUtils::k_FlagOpenCollapseHeader))
		{
			float elemPadding = 15.f;
			for(size_t i = 0; i < materials.size(); i++)
			{
				std::string elemTitle("Element ");
				elemTitle.append(std::to_string(i));
				bool bMatOpen = ImGui::TreeNodeEx(elemTitle.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | 
												                     ImGuiTreeNodeFlags_OpenOnDoubleClick/* | ImGuiTreeNodeFlags_DefaultOpen*/); 
				if(bMatOpen)
				{
					DrawMaterialElem(materials[i]);
					DrawMaterialBall(materials[i]);					
					ImGui::TreePop();
					ImGui::Spacing();
				}
			}
		}
	}

	void DetailPanel::DrawMaterialElem(const Ref<Material>& elem)
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Shader     ");
		ImGui::SameLine();
		auto remain = ImGui::GetContentRegionAvail();
		ImGui::PushItemWidth(remain.x);
		std::string name = ShaderUtils::GetName(elem->GetShaderSpec());
		ImGui::InputText("##Material_Shader", const_cast<char*>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();

		if(elem->Is<StandardMaterial>())
		{
			DrawStandardMaterial(elem->As<StandardMaterial>());
		}

		ImGui::Text("Lighting");
		int32_t mode = elem->GetShadowCastMode();
		const char* sdCastMode[ShadowCastMode_Num] = { "Off", "On", "Two Sided", "Shadow Only" };
		ImGui::AlignTextToFramePadding();
		ImGui::Text("CastShadow ");
		ImGui::SameLine();
		ImGui::Combo("##ShadowCastMode", &mode, sdCastMode, ShadowCastMode_Num);
		elem->SetShadowCastMode(ShadowCastMode(mode));
	}

	void DetailPanel::DrawMaterialBall(const Ref<Material>& elem)
	{
		m_MaterialBall->SetMaterial(elem);
		m_MaterialBall->OnDraw();

		auto avail = ImGui::GetContentRegionAvail();
		ImGui::BeginChild("MaterialBall", ImVec2(avail.x, avail.y), ImGuiChildFlags_None);
		float aspect0 = avail.x / avail.y;
		float aspect1 = m_MaterialBall->GetAspect();
		glm::vec2 imgSize;
		//Let image always fills the window size
		if(aspect0 > aspect1)
		{
			imgSize.x = aspect1 * avail.y;
			imgSize.y = avail.y;
		}
		else
		{
			imgSize.x = avail.x;
			imgSize.y = avail.x / aspect1;
		}
		ImGuiUtils::DrawImage(m_MaterialBall->GetColorBufferId(), imgSize, glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f), 0.f, 0.f);
		ImGui::EndChild();
	}

	void DetailPanel::DrawStandardMaterial(StandardMaterial* material)
	{		
		{
			auto albedoMap = material->GetAlbedoMap()->GetTexture();
			auto albedo = material->GetAlbedo();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("AlbedoMap  ");
			ImGui::SameLine();			
			auto textSize = ImGui::CalcTextSize("Albedo");
			bool click = ImGuiUtils::DrawImageButton("##AlbedoMap", albedoMap->Id(),
													 glm::vec2(textSize.y, textSize.y),
													 glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f),
													 glm::vec4(0.f, 0.f, 0.f, 1.f),
													 glm::vec4(1.f), albedoMap->Name().c_str());

			OpenImageModal(click, "AlbedoMap", albedoMap->Id());

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Albedo     ");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Albedo", glm::value_ptr(albedo));
			material->SetAlbedo(albedo);
		}

		{
			auto metallicRoughnessMap = material->GetMetallicRoughnessMap()->GetTexture();
			auto metallic = material->GetMetallic();
			auto roughness = material->GetRoughness();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("MetalRough ");
			ImGui::SameLine();
			auto textSize = ImGui::CalcTextSize("Metallic");
			bool click = ImGuiUtils::DrawImageButton("##Metallic-Rougness", metallicRoughnessMap->Id(),
										 glm::vec2(textSize.y, textSize.y),
										 glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f),
										 glm::vec4(0.f, 0.f, 0.f, 1.f),
										 glm::vec4(1.f), metallicRoughnessMap->Name().c_str());

			OpenImageModal(click, "MetallicRoughness", metallicRoughnessMap->Id());			

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Metallic   ");
			ImGui::SameLine();
			ImGui::SliderFloat("##Metallic", &metallic, 0.f, 1.f, "%.3f");
			material->SetMetallic(metallic);

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Roughness  ");
			ImGui::SameLine();
			ImGui::SliderFloat("##Roughness", &roughness, 0.f, 1.f, "%.3f");
			material->SetRoughness(roughness);

			if(material->HasFlag(ShaderPermutationFlag::USE_CLEARCOAT))
			{	
				auto clearCoat = material->GetClearCoatFactor();
				auto clearCoatRoughness = material->GetClearCoatRoughnessFactor();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("ClearCoat  ");
				ImGui::SameLine();
				ImGui::SliderFloat("##ClearCoatFactor", &clearCoat, 0.f, 1.f, "%.3f");
				material->SetClearCoatFactor(clearCoat);

				ImGui::AlignTextToFramePadding();
				ImGui::Text("ClearCoatRoughness");
				ImGui::SameLine();
				ImGui::SliderFloat("##ClearCoatRoughness", &clearCoatRoughness, 0.f, 1.f, "%.3f");
				material->SetClearCoatRoughnessFactor(clearCoatRoughness);
			}
		}
	}

	void DetailPanel::OpenImageModal(bool bOpen, const char* title, int32_t texId)
	{
		if(bOpen)
		{
			m_ModalOpenStat[title] = true;
		}

		if(m_ModalOpenStat[title])
		{
			m_ModalOpenStat[title] = ImGuiUtils::DrawImageModal(title, texId);
		}
	}
}