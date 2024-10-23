#include "ImGuiUtils.h"
#include "MaterialInspector.h"

#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/node/Node.h"
#include "scene/node/LightNode.h"
#include "scene/node/MeshNode.h"
#include "scene/node/SpriteNode.h"

#include "gfx/rhi/Texture.h"
#include "gfx/resource/material/PbrMaterial.h"
#include "gfx/resource/material/SpriteMaterial.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImGuiUtils"

namespace pio
{
	const int32_t ImGuiUtils::k_FlagCollapseHeader = ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	const int32_t ImGuiUtils::k_FlagSelectedTreeNode = ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	const int32_t ImGuiUtils::k_FlagTreeLeaf = ImGuiTreeNodeFlags_Leaf |
		ImGuiTreeNodeFlags_NoTreePushOnOpen;

	const int32_t ImGuiUtils::k_FlagCommonWindow = ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoScrollWithMouse | 
		ImGuiWindowFlags_NoCollapse;

	void ImGuiUtils::DrawImage(int32_t texId, const glm::vec2& imgSize, const glm::vec2& ltTexCoord, const glm::vec2& rbTexCoord, float rowWidth, float indent)
	{
		bool useIndent = !Math::IsZero(indent);
		bool useRowWidth = !Math::IsZero(rowWidth);

		if(useIndent)
		{
			ImGui::Indent(indent);
		}

		if(useRowWidth)
		{
			ImGui::PushItemWidth(rowWidth);
		}

		ImTextureID id = (int32_t*)texId;
		ImVec2 uv_min = ImVec2(ltTexCoord.x, ltTexCoord.y); // Top-left
		ImVec2 uv_max = ImVec2(rbTexCoord.x, rbTexCoord.y); // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
		ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
		ImGui::Image(id, ImVec2(imgSize.x, imgSize.y), uv_min, uv_max, tint_col, border_col);

		if(useRowWidth)
		{
			ImGui::PopItemWidth();
		}

		if(useIndent)
		{
			ImGui::Unindent(indent);
		}
	}

	void ImGuiUtils::HelpMaker(const char* msg)
	{
		ImGui::TextDisabled("(?)");
		if(ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(msg);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	bool ImGuiUtils::ItemBeingClicked()
	{
		return ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen();
	}

	void ImGuiUtils::ShowHierarchy(const std::vector<Ref<Node>>& nodes, uint32_t& selectIdx)
	{
		if(nodes.empty())
			return;

		uint32_t clickIdx = InvalidId;
		if(selectIdx == InvalidId && nodes[0]->bShowInInspector())
		{
			selectIdx = nodes[0]->idx();
		}
		ShowRelation(nodes, selectIdx, clickIdx);
	}

	void ImGuiUtils::ShowNode(const Ref<Node>& node)
	{	
		auto type = node->nodeType();
		MaterialInspector::Get()->setVisible(type == NodeType::Mesh);
		switch (type)
		{
			case NodeType::Camera:				
				DrawCameraPanel(node);
				break;
			case NodeType::Light:
				MaterialInspector::Get()->setVisible(false);
				DrawLightPanel(node);
				break;
			case NodeType::Mesh:				
				DrawMeshPanel(node);
				break;
			case NodeType::Sprite:				
				DrawSpritePanel(node);
				break;
			default:
				break;
		}
	}

	void ImGuiUtils::ShowRelation(const std::vector<Ref<Node>>& nodes, uint32_t& selectIdx, uint32_t& clickIdx)
	{
		for(size_t i = 0; i < nodes.size(); i++)
		{
			if (!nodes[i]->bShowInInspector())
			{
				continue;
			}
			if (selectIdx == InvalidId)
			{
				selectIdx = nodes[i]->idx();
			}
			ImGuiTreeNodeFlags tnFlags = ImGuiUtils::k_FlagSelectedTreeNode;
			tnFlags |= (selectIdx == nodes[i]->idx()) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
			bool hasChild = !nodes[i]->children().empty();
			if(!hasChild)
			{
				tnFlags |= ImGuiUtils::k_FlagTreeLeaf;
			}
			bool bOpen = ImGui::TreeNodeEx(nodes[i]->name().c_str(), tnFlags);
			if(ImGuiUtils::ItemBeingClicked())
			{
				clickIdx = nodes[i]->idx();
			}
			if(bOpen && hasChild)
			{
				ShowRelation(nodes[i]->children(), selectIdx, clickIdx);
				ImGui::TreePop();
			}
			if(clickIdx != InvalidId)
			{
				selectIdx = clickIdx;
			}
		}
	}

	void ImGuiUtils::DrawCameraPanel(const Ref<Node>& node)
	{
		if(!node || !node->has<CameraComponent>())
		{
			return;
		}
		auto* cameraComp = node->getComponent<CameraComponent>();
		auto* transComp = node->getComponent<TransformComponent>();

		DrawTransformPanel(transComp);

		if(ImGui::CollapsingHeader("Camera", ImGuiUtils::k_FlagCollapseHeader))
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Near      ");
			ImGui::SameLine();
			ImGui::DragFloat("##Near", &cameraComp->Near, 1.f, 0.1, 100.f, "%.1f");

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Far       ");
			ImGui::SameLine();
			ImGui::DragFloat("##Far", &cameraComp->Far, 1.f, 0.1, 1000.f, "%.1f");

			int prjType{ cameraComp->PrjType };
			const char* items[2]{ "Perspective", "Orthographic" };
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Projection");
			ImGui::SameLine();
			ImGui::Combo("##Prj_type", &prjType, items, ProjectionType_Num);
			cameraComp->PrjType = ProjectionType(prjType);

			if(prjType == ProjectionType_Perspective)
			{				
				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Fov       ");
				ImGui::SameLine();
				ImGui::DragFloat("##FOV", &cameraComp->Fov, 1.f, 0.1, 179.f, "%.1f");				
			}
			else
			{				
				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Size      ");
				ImGui::SameLine();
				ImGui::DragFloat("##Size", &cameraComp->Size, 0.1f, 0.1, 100.f, "%.1f");
			}
		}
	}

	void ImGuiUtils::DrawLightPanel(const Ref<Node>& node)
	{
		auto* lightNode = node->as<LightNode>();
		if (!lightNode)
			return;

		auto* transComp = node->getComponent<TransformComponent>();
		DrawTransformPanel(transComp);

		auto type = lightNode->lightType();
		if (type == LightType::DirectionLight)
		{				
			if (ImGui::CollapsingHeader("DirectionalLight", ImGuiUtils::k_FlagCollapseHeader))
			{
				auto* lightComp = node->getComponent<DirectionalLightComponent>();

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Color      ");
				ImGui::SameLine();
				ImGui::ColorEdit3("##Light_Color", &lightComp->Color.r);

				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Intensity  ");
				ImGui::SameLine();
				ImGui::DragFloat("##Light_Intensity", &lightComp->Intensity, 0.02f, 0.1f, 50.f, "%.2f", 0);

				const char* shadowModeNames[ShadowMode_Num] = { "No Shadows", "Hard", "Soft", "Soft_2X", "Soft_4X" };
				ImGui::AlignTextToFramePadding();
				ImGui::Text(" Shadow Type");
				ImGui::SameLine();
				ImGui::Combo("##Shadow_Type", &lightComp->ShadowMode, shadowModeNames, ShadowMode_Num);
				lightComp->CastShadow = lightComp->ShadowMode != ShadowMode_None;
				if(lightComp->CastShadow)
				{
					ImGui::AlignTextToFramePadding();
					ImGui::Text("  Realtime Shadows");

					ImGui::AlignTextToFramePadding();
					ImGui::Text("  Strength   ");
					ImGui::SameLine();
					auto remain = ImGui::GetContentRegionAvail();
					ImGui::PushItemWidth(remain.x);
					ImGui::DragFloat("##Sd_Intensity", &lightComp->ShadowIntensity, 0.02f, 0.f, 1.f, "%.2f");
					ImGui::PopItemWidth();

					ImGui::AlignTextToFramePadding();
					ImGui::Text("  Bias       ");
					ImGui::SameLine();
					remain = ImGui::GetContentRegionAvail();
					ImGui::PushItemWidth(remain.x);
					ImGui::DragFloat("##Sd_Bias", &lightComp->Bias, 0.00001f, 0.00001f, 1.f, "%.5f");
					ImGui::PopItemWidth();

					ImGui::AlignTextToFramePadding();
					ImGui::Text("  Normal Bias");
					ImGui::SameLine();
					HelpMaker("Normal Bias has not been implemented");
					ImGui::SameLine();
					remain = ImGui::GetContentRegionAvail();
					ImGui::PushItemWidth(remain.x);
					ImGui::DragFloat("##Normal_Bias", &lightComp->NormalBias, 0.01f, 0.f, 3.f, "%.2f");
					ImGui::PopItemWidth();
				}
			}
		}
	}

	void ImGuiUtils::DrawMeshPanel(const Ref<Node>& node)
	{
		auto* meshNode = node->as<MeshNode>();
		if (!meshNode)
			return;

		auto* transComp = meshNode->getComponent<TransformComponent>();
		DrawTransformPanel(transComp);

		if (ImGui::CollapsingHeader("MeshFilter", ImGuiUtils::k_FlagCollapseHeader))
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Mesh   ");
			ImGui::SameLine();
			auto remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			std::string name = GetMeshTypeStr(meshNode->meshType());
			ImGui::InputText("##MeshType Name", const_cast<char*>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
		}

		bool bMatTreeNodeOpen = false;
		if (ImGui::CollapsingHeader("MeshRenderer", ImGuiUtils::k_FlagCollapseHeader))
		{
			auto* meshRender = meshNode->getComponent<MeshRenderer>();
			bMatTreeNodeOpen = ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);
			if (bMatTreeNodeOpen)
			{				
				auto material = AssetMgr::GetRuntimeAsset<Material>(meshRender->MatHnd);
				MaterialInspector::Get()->setMaterial(material);				
				auto shaderSpec = material->spec();
				auto renderingMode = material->renderingMode();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Shader        ");
				ImGui::SameLine();
				auto remain = ImGui::GetContentRegionAvail();
				ImGui::PushItemWidth(remain.x);
				std::string name = GetShaderSpecStr(shaderSpec);
				ImGui::InputText("##Material_Shader", const_cast<char*>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Rendering Mode");
				ImGui::SameLine();
				remain = ImGui::GetContentRegionAvail();
				ImGui::PushItemWidth(remain.x);
				name = GetRenderingModeStr(renderingMode);
				ImGui::InputText("##Material_RenderingMode", const_cast<char*>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();

				switch (shaderSpec)
				{
					case ShaderSpec_Standard:
						DrawStandardMaterial(material);
						break;
					default:
						break;
				}

				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNodeEx("Lighting", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
			{
				int32_t mode = meshRender->SdCastMode;
				const char* sdCastMode[ShadowCastMode_Num] = { "Off", "On", "Two Sided", "Shadow Only" };				
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Cast Shadows  ");
				ImGui::SameLine();				
				ImGui::Combo("##ShadowCastMode", &mode, sdCastMode, ShadowCastMode_Num);
				meshRender->SdCastMode = ShadowCastMode(mode);

				ImGui::TreePop();
				ImGui::Spacing();
			}
		}

		if (bMatTreeNodeOpen)
		{
			MaterialInspector::Get()->onDraw();
			auto avail = ImGui::GetContentRegionAvail();
			ImGui::BeginChild("MaterialInspector", ImVec2(avail.x, avail.y), ImGuiChildFlags_None);
			float aspect0 = avail.x / avail.y;
			float aspect1 = MaterialInspector::Get()->aspect();
			glm::vec2 imgSize;
			//Let image always fills the window size
			if (aspect0 < aspect1)
			{
				imgSize.x = aspect1 * avail.y;
				imgSize.y = avail.y;
			}
			else
			{
				imgSize.x = avail.x;
				imgSize.y = avail.x / aspect1;
			}
			ImGuiUtils::DrawImage(MaterialInspector::Get()->colorBuffer()->id(), imgSize, glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f), 0.f, 0.f);
			ImGui::EndChild();
		}
	}

	void ImGuiUtils::DrawSpritePanel(const Ref<Node>& node)
	{
		auto spriteNode = node->as<SpriteNode>();
		if (!spriteNode)
			return;

		auto* transComp = spriteNode->getComponent<TransformComponent>();
		DrawTransformPanel(transComp);

		auto* spriteRender = spriteNode->getComponent<SpriteRenderer>();
		if (ImGui::CollapsingHeader("Sprite Renderer", ImGuiUtils::k_FlagCollapseHeader))
		{
			auto material = AssetMgr::GetRuntimeAsset<SpriteMaterial>(spriteRender->MatHnd);
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Sprite   ");
			ImGui::SameLine();
			auto remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			std::string name = "Square";
			ImGui::InputText("##Sprite Type", const_cast<char*>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Color    ");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Color", &spriteRender->Color.r);

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Flip     ");
			ImGui::SameLine();
			ImGui::Checkbox("X", &spriteRender->FlipX);
			ImGui::SameLine();
			ImGui::Checkbox("Y", &spriteRender->FlipY);

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Material ");
			ImGui::SameLine();
			remain = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(remain.x);
			std::string matName = material->name();
			ImGui::InputText("##Sprite Material", const_cast<char*>(matName.c_str()), matName.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
		}
	}

	void ImGuiUtils::DrawStandardMaterial(Ref<Material>& material)
	{
		auto* pbrMaterial = material->as<PbrMaterial>();
		if (!pbrMaterial)
		{
			return;
		}
		glm::vec3 albedo = pbrMaterial->getAlbedo();
		auto albedoTex = pbrMaterial->getAlbedoTexture();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Albedo        ");
		ImGui::SameLine();
		if (albedoTex)
		{
			auto textSize = ImGui::CalcTextSize("Albedo");
			ImVec2 size = ImVec2(textSize.y, textSize.y);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));			
			ImVec2 uv0 = ImVec2(0.0f, 1.0f);                            
			ImVec2 uv1 = ImVec2(1.f, 0.f);    
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);            
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImTextureID id = (int32_t*)albedoTex->id();
			ImGui::ImageButton("##albedo_tex", id, size, uv0, uv1, bg_col, tint_col);
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip(albedoTex->name().c_str(), ImGui::GetStyle().HoverDelayShort);
			ImGui::SameLine();
		}
		ImGui::ColorEdit3("##Albedo", &albedo.r);		
		pbrMaterial->setAlbedo(albedo);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Metallic      ");
		ImGui::SameLine();
		auto metalRough = pbrMaterial->getMetallicRoughness();
		if (metalRough)
		{
			auto textSize = ImGui::CalcTextSize("Metallic");			
			ImVec2 size = ImVec2(textSize.y, textSize.y);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
			ImVec2 uv0 = ImVec2(0.0f, 1.0f);
			ImVec2 uv1 = ImVec2(1.f, 0.f);
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImTextureID id = (int32_t*)metalRough->id();
			ImGui::ImageButton("##metalRough_tex", id, size, uv0, uv1, bg_col, tint_col);
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip(metalRough->name().c_str(), ImGui::GetStyle().HoverDelayShort);			
		}
		else
		{
			float metallic = pbrMaterial->getMetallic();			
			ImGui::SliderFloat("##Metallic", &metallic, 0.f, 1.f, "%.3f");
			pbrMaterial->setMetallic(metallic);
		}		

		float smoothness = pbrMaterial->getSmoothness();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Smoothness    ");
		ImGui::SameLine();		
		ImGui::SliderFloat("##Smoothness", &smoothness, 0.f, 1.f, "%.3f");
		pbrMaterial->setSmoothness(smoothness);
	}

	void ImGuiUtils::DrawTransformPanel(TransformComponent* comp)
	{
		if (!comp)
			return;

		if(ImGui::CollapsingHeader("Transform", ImGuiUtils::k_FlagCollapseHeader))
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Position");
			ImGui::SameLine();
			ImGui::DragFloat3("##Position", glm::value_ptr(comp->Position), 0.05f, -100.f, 100.f, "%.1f");

			ImGui::AlignTextToFramePadding();
			ImGui::Text(" Rotation");
			ImGui::SameLine();
			ImGui::DragFloat3("##Rotation", glm::value_ptr(comp->Rotation), 0.1f, -360.f, 360.f, "%.1f");

			ImGui::AlignTextToFramePadding();
			//Enough text padding for text alignment
			ImGui::Text(" Scale   ");
			ImGui::SameLine();
			ImGui::DragFloat3("##Scale", glm::value_ptr(comp->Scale), 0.1f, 0.f, 10.f, "%.1f");
		}
	}
}