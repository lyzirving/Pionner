#include "EditorLayer.h"

#include "Application.h"

#include "asset/AssetsManager.h"

#include "scene/Scene.h"
#include "scene/Components.h"
#include "gfx/core/Skybox.h"

#include "animation/Animation.h"
#include "animation/Animator.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/rhi/Texture.h"
#include "gfx/struct/Mesh.h"
#include "gfx/struct/MaterialAsset.h"
#include "gfx/debug/GDebugger.h"

#include "ui/ImGuiUtils.h"
#include "ui/panel/UiPanel.h"
#include "ui/MotionController.h"

#include "core/math/SphereCoord.h"

#include <imgui.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EditorLayer"

namespace pio
{
	static void DealSelectionChange(Ref<Entity> &cur, Ref<Entity> &last)
	{
		EntityClass type(EntityClass::None);

		if (last)
		{
			type = last->getClass();
			if (type == EntityClass::Mesh || type == EntityClass::MeshSource)
			{
				MotionController::SelectObj3D(nullptr);
				last->setSelection(false);
			}
			else if (last->hasComponent<SpriteComponent>())
			{
				MotionController::SelectSprite(nullptr);
			}
		}

		if (cur)
		{
			type = cur->getClass();
			if (type == EntityClass::Mesh || type == EntityClass::MeshSource)
			{
				MotionController::SelectObj3D(cur);
				cur->setSelection(true);
			}
			else if (cur->hasComponent<SpriteComponent>())
			{
				MotionController::SelectSprite(cur);
			}
		}
	}

	EditorLayer::EditorLayer(const LayoutParams &param)
		: Layer(param, "EditorLayer")
	{
	}

	void EditorLayer::onAttach()
	{				
		onWindowSizeChange(Application::MainWindow()->width(),
						   Application::MainWindow()->height());
	}

	void EditorLayer::onDetach()
	{		
	}

	void EditorLayer::onUpdateUI(const Timestep &ts)
	{
		const std::string &name = m_name;
		LayoutParams &param = m_layoutParam;
		Registry *registry = s_registry;
		Ref<Entity> &lastSelect = m_selectedEnt;

		Renderer::SubmitRC([this, name, param, registry, &lastSelect]() mutable
		{
			PIO_ASSERT_RETURN(ImGui::GetCurrentContext() != nullptr, "EditorLayer::onUpdate: Missing ImGui context");

			ImGuiWindowFlags flags = ImGuiWindowFlags_None;
			flags |= ImGuiWindowFlags_NoMove;
			flags |= ImGuiWindowFlags_NoResize;
			flags |= ImGuiWindowFlags_NoTitleBar;

			ImGui::SetNextWindowPos(ImVec2(param.Position.Left, param.Position.Top), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(param.Viewport.w() + 1, param.Viewport.h() + 1), ImGuiCond_Always);

			if (!ImGui::Begin(name.c_str(), nullptr, flags))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			const float rowWidth = m_layoutParam.Viewport.w();
			RendererConfig &config = Renderer::GetConfig();
			ImGui::PushItemWidth(rowWidth);
			ImGui::LabelText("##FPS", "FPS[%.1f], FrameTime[%lu]ms", config.FPS, config.FrameTime);
			ImGui::PopItemWidth();

			EditorComponent &editorComp = registry->getSingleton<EditorComponent>();
			if (editorComp.SelectedEntIndex != NullIndex)
			{
				Ref<Entity> select = registry->getCachedEntity(editorComp.SelectedEntIndex);
				if (lastSelect != select)
				{
					DealSelectionChange(select, lastSelect);
					lastSelect = select;
				}
				switch (select->getClass())
				{
					case EntityClass::Scene:
						onDrawScenePanel(select);
						break;
					case EntityClass::DirectionalLight:
					case EntityClass::PointLight:
						UiPanel::DrawLightPanel(select);					
						break;
					case EntityClass::MeshSource:
						onDrawMeshSourcePanel(select);
						break;
					case EntityClass::Mesh:
						onDrawMeshPanel(select);
						break;
					case EntityClass::Camera:
						UiPanel::DrawCameraPanel(select);
						break;
					default:
						break;
				}
			}
			ImGui::End();
			//ImGui::ShowDemoWindow();
		});
	}

	void EditorLayer::onWindowSizeChange(uint32_t width, uint32_t height)
	{
		m_layoutParam.calculate(width, height);
	}

	void EditorLayer::onDrawScenePanel(Ref<Entity> &ent)
	{
		if (ent && ent->hasComponent<SceneComponent>())
		{
			auto scene = AssetsManager::GetRuntimeAsset<Scene>(ent->getComponent<SceneComponent>().Handle);
			const std::string &name = ent->getName();
			const float rowWidth = m_layoutParam.Viewport.w();
			
			ImGui::PushItemWidth(rowWidth);
			ImGui::InputText("##scene_name", const_cast<char *>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			if (ImGui::CollapsingHeader("Physics##Scene", ImGuiUtils::Flag_Collapse_Header))
			{
				bool bSimulate = scene->bSimulate();
				ImGui::Checkbox("Simulate##Scene_Physics", &bSimulate);
				scene->setSimulate(bSimulate);
				ImGui::SameLine();
				bool raycastSwitch = Renderer::GetConfig().Debugger.Raycast;
				ImGui::Checkbox("Raycast##Scene_Physics", &raycastSwitch);
				if (raycastSwitch != Renderer::GetConfig().Debugger.Raycast)
				{
					Renderer::GetConfig().Debugger.Raycast = raycastSwitch;
					if (!raycastSwitch)
					{
						GDebugger::Get()->clear(GDebug_Line);
					}
				}
			}
		}
	}

	void EditorLayer::onDrawMeshSourcePanel(Ref<Entity> &ent)
	{
		if (ent && ent->hasComponent<MeshSourceComponent>())
		{
			auto &meshSrcComp = ent->getComponent<MeshSourceComponent>();
			auto meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshSrcComp.SourceHandle);
			if (meshSrc)
			{
				const float rowWidth = m_layoutParam.Viewport.w();
				bool bVisible = meshSrcComp.Visible;
				UiPanel::DrawNamePanel("##meshSource_name", meshSrc->getName(), "##meshSource_visibility", bVisible, rowWidth);
				if (meshSrcComp.Visible != bVisible)
				{
					meshSrcComp.Visible = bVisible;
					const std::vector<Submesh>& submeshes = meshSrc->getSubmeshes();
					for (uint32_t i = 0; i < submeshes.size(); i++)
					{
						if (submeshes[i].Ent->hasComponent<MeshComponent>())
						{
							submeshes[i].Ent->getComponent<MeshComponent>().Visible = meshSrcComp.Visible;
						}
						else if (submeshes[i].Ent->hasComponent<StaticMeshComponent>())
						{
							submeshes[i].Ent->getComponent<StaticMeshComponent>().Visible = meshSrcComp.Visible;
						}
					}
				}
				UiPanel::DrawTransformPanel(meshSrc->GlobalPose);			
				if (ent->hasComponent<AnimationComponent>() && meshSrc->hasAnimation())
				{
					auto& comp = ent->getComponent<AnimationComponent>();
					const std::vector<Ref<Animation>>& animations = meshSrc->getAnimation();

					uint32_t animCnt = animations.size();
					std::vector<const char*> aniNames;
					aniNames.reserve(animCnt);
					for (uint32_t i = 0; i < animCnt; i++)
						aniNames.emplace_back(animations[i]->getName().c_str());
					if (ImGui::CollapsingHeader("Animation##MeshSource", ImGuiUtils::Flag_Collapse_Header) && animCnt > 0)
					{
						ImGui::Checkbox("GPU Skinning##MeshSource", &comp.GPUSkinning);
						ImGui::Combo("##AnimationMeshSource", &comp.Selection, aniNames.data(), animCnt, 3);
					}
				}
			}
		}
	}

	void EditorLayer::onDrawMeshPanel(Ref<Entity> &ent)
	{
		if (!ent) return;

		Ref<MeshBase> meshBase{ nullptr };
		uint32_t submeshIdx{ NullIndex };
		bool bVisible{ false }, bVisibleUI{ false };

		if (ent->hasComponent<MeshComponent>())
		{
			auto &meshComp = ent->getComponent<MeshComponent>();
			meshBase = AssetsManager::GetRuntimeAsset<MeshBase>(meshComp.Handle);
			submeshIdx = meshComp.SubmeshIndex;
			bVisible = bVisibleUI = meshComp.Visible;
		}
		else if (ent->hasComponent<StaticMeshComponent>())
		{
			auto &meshComp = ent->getComponent<StaticMeshComponent>();
			meshBase = AssetsManager::GetRuntimeAsset<MeshBase>(meshComp.Handle);
			submeshIdx = meshComp.SubmeshIndex;
			bVisible = bVisibleUI = meshComp.Visible;
		}

		if (meshBase)
		{
			const Submesh &submesh = meshBase->getMeshSource()->getSubmeshes()[submeshIdx];
			const float rowWidth = m_layoutParam.Viewport.w();
			UiPanel::DrawNamePanel("##submesh_name", submesh.MeshName, "##mesh_visibility", bVisibleUI, rowWidth);
			UiPanel::DrawTransformPanel(ent);

			Ref<MaterialTable> mt = meshBase->getMaterialTable();
			Ref<MaterialAsset> ma = AssetsManager::GetRuntimeAsset<MaterialAsset>((*mt)[submesh.MaterialIndex]);
			if (ma && ImGui::CollapsingHeader("Material##Mesh", ImGuiUtils::Flag_Collapse_Header))
			{
				float metallic, metallic_ui;
				float roughness, roughness_ui;

				Ref<Texture2D> whiteTexture = Renderer::GetWhiteTexture();
				Ref<Texture2D> blackTexture = Renderer::GetBlackTexture();

				std::string name = ma->getName();
				ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);

				metallic = metallic_ui = ma->getMetalness();
				roughness = roughness_ui = ma->getRoughness();

				ImGui::PushItemWidth(rowWidth);
				ImGui::InputText("##material_name", const_cast<char *>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();

				if (ImGui::TreeNode("Albedo##Mesh"))
				{			
					float imgWidth = ImGui::GetContentRegionAvail().x;
					Ref<Texture2D> tx = ma->getAlbedoMap();
					bool bTexture = tx && tx != whiteTexture && tx != blackTexture;
					std::string msg = bTexture ? tx->getName() : "None Texture";

					ImGui::TextWrapped(msg.c_str());
					if (bTexture) { ImGuiUtils::DrawImage(tx->getId(), glm::vec2(imgWidth)); }

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("MetallicRoughness##Mesh"))
				{
					float imgWidth = ImGui::GetContentRegionAvail().x;
					Ref<Texture2D> tx = ma->getMetallicRoughnessMap();	
					bool bTexture = tx && tx != whiteTexture && tx != blackTexture;
					std::string msg = bTexture ? tx->getName() : "None Texture";

					ImGui::TextWrapped(msg.c_str());
					if (bTexture) { ImGuiUtils::DrawImage(tx->getId(), glm::vec2(imgWidth)); }

					ImGui::AlignTextToFramePadding();
					ImGui::Text("Metallic ");
					ImGui::SameLine();
					ImGui::DragFloat("##Material_Metallic", &metallic_ui, 0.005f, 0.0f, 1.0f, "%.3f");

					ImGui::AlignTextToFramePadding();
					ImGui::Text("Roughness");
					ImGui::SameLine();
					ImGui::DragFloat("##Material_Roughness", &roughness_ui, 0.005f, 0.0f, 1.0f, "%.3f");

					ImGui::TreePop();
				}	

				if (ImGui::TreeNode("Emission##Mesh"))
				{
					float imgWidth = ImGui::GetContentRegionAvail().x;
					Ref<Texture2D> tx = ma->getEmissionMap();
					bool bTexture = tx && tx != whiteTexture && tx != blackTexture;
					std::string msg = bTexture ? tx->getName() : "None Texture";

					ImGui::TextWrapped(msg.c_str());										
					if (bTexture) { ImGuiUtils::DrawImage(tx->getId(), glm::vec2(imgWidth)); }

					bool emEnable{ false };// [TODO]: enable glow effect					
					ImGui::Checkbox("Enabled##Mesh_Mat_Emission", &emEnable);

					ImGui::TreePop();
				}

				if (!Math::Equal(metallic, metallic_ui)) 
					ma->setMetalness(metallic_ui);

				if (!Math::Equal(roughness, roughness_ui))
					ma->setRoughness(roughness_ui);
			}
		}

		if (bVisible != bVisibleUI)
		{
			if (ent->hasComponent<MeshComponent>())
			{
				auto &meshComp = ent->getComponent<MeshComponent>();
				meshComp.Visible = bVisibleUI;
			}
			else if (ent->hasComponent<StaticMeshComponent>())
			{
				auto &meshComp = ent->getComponent<StaticMeshComponent>();
				meshComp.Visible = bVisibleUI;
			}
		}
	}
}