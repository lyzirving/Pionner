#include "EditorLayer.h"

#include "Application.h"

#include "asset/AssetsManager.h"

#include "scene/Components.h"
#include "scene/Skybox.h"

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

	EditorLayer::EditorLayer(const WindowLayoutParams &param)
		: Layer(param, "EditorLayer")
	{
	}

	void EditorLayer::onAttach()
	{		
		m_mainCameraEnt = s_registry->mainCameraEnt();
		m_sceneEnt = s_registry->mainSceneEnt();

		onWindowSizeChange(Application::MainWindow()->getWidth(),
						   Application::MainWindow()->getHeight());
	}

	void EditorLayer::onDetach()
	{
		m_mainCameraEnt.reset();
		m_sceneEnt.reset();
	}

	void EditorLayer::onUpdateUI(const Timestep &ts)
	{
		const std::string &name = m_name;
		WindowLayoutParams &param = m_layoutParam;
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
			ImGui::SetNextWindowSize(ImVec2(param.Viewport.Width + 1, param.Viewport.Height + 1), ImGuiCond_Always);

			if (!ImGui::Begin(name.c_str(), nullptr, flags))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			const float rowWidth = m_layoutParam.Viewport.Width;
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
						onDrawDistantLightPanel(select);
						break;
					case EntityClass::PointLight:
						onDrawPointLightPanel(select);
						break;
					case EntityClass::MeshSource:
						onDrawMeshSourcePanel(select);
						break;
					case EntityClass::Mesh:
						onDrawMeshPanel(select);
						break;
					case EntityClass::Camera:
						onDrawCameraPanel(select);
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
			auto &sceneComp = ent->getComponent<SceneComponent>();
			const std::string &name = ent->getName();
			const float rowWidth = m_layoutParam.Viewport.Width;
			
			ImGui::PushItemWidth(rowWidth);
			ImGui::InputText("##scene_name", const_cast<char *>(name.c_str()), name.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			if (ImGui::CollapsingHeader("Physics##Scene", ImGuiUtils::Flag_Collapse_Header))
			{
				ImGui::Checkbox("Simulate##Scene_Physics", &sceneComp.Simulate);
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

			if (ImGui::CollapsingHeader("Skybox##Scene", ImGuiUtils::Flag_Collapse_Header))
			{				
				Ref<Skybox> sk = AssetsManager::GetRuntimeAsset<Skybox>(sceneComp.Skybox);
				const auto &name = sk->getName();
				ImGui::Text("Env map: %s", name.c_str());

				float intensity = sk->getIntensity();
				ImGui::SliderFloat("Intensity##Skybox", &intensity, 0.0001f, 0.3f, "%.4f");
				sk->setIntensity(intensity);
			}
		}
	}

	void EditorLayer::onDrawDistantLightPanel(Ref<Entity> &ent)
	{
		/*{
			bool bVisible = true;
			UiPanel::DrawNamePanel("##light_name", ent->getName(), "##light_visibility", bVisible);
			UiPanel::DrawLightPanel(ent);
			return;
		}*/

		if (ent && ent->hasComponent<DirectionalLightComponent>())
		{			
			auto &lightComp = ent->getComponent<DirectionalLightComponent>();
			auto &transComp = ent->getComponent<TransformComponent>();

			const float rowWidth = m_layoutParam.Viewport.Width;
			bool bVisible{ true };
			UiPanel::DrawNamePanel("##DirectionalLight_name", ent->getName(), "##DirectionalLight_visibility", bVisible, rowWidth);
			UiPanel::DrawTransformPanel(ent);

			ImGui::Checkbox("CastShadow##DistantLight", &lightComp.CastShadow);
			ImGui::Combo("PCF", &lightComp.SdMode, ShadowModeNames, ShadowMode_Num, 3);
			ImGui::DragFloat("Bias##DistantLight", &lightComp.Bias, 0.00001f, 0.0001f, 0.1f, "%.5f");

			glm::vec3 dir = lightComp.Direction;
			ImGui::DragFloat3("Direction##DirectionalLight", &dir.x, 0.01f, -1.f, 1.f, "%.2f");
			if (dir != lightComp.Direction)
			{
				transComp.Transform.Euler.invalidate();
				lightComp.Direction = glm::normalize(dir);
			}

			ImGui::DragFloat3("Radiance##DirectionalLight", glm::value_ptr(lightComp.Radiance), 0.1f, 0.f, 1000.f, "%.1f");
			ImGui::SliderFloat("Intensity##DirectionalLight", &lightComp.Intensity, 0.01f, 2.f, "%.2f", 0);
		}
	}

	void EditorLayer::onDrawPointLightPanel(Ref<Entity> &ent)
	{
		if (ent && ent->hasComponent<PointLightComponent>())
		{
			auto& lightComp = ent->getComponent<PointLightComponent>();	
			auto &transComp = ent->getComponent<TransformComponent>();
			const auto& name = ent->getName();
			std::string nLabel = (std::stringstream() << "##" << ent->getName() << "_name").str();
			std::string vLabel = (std::stringstream() << "##" << ent->getName() << "_visibility").str();

			const float rowWidth = m_layoutParam.Viewport.Width;
			bool bVisible{ true };//TODO
			UiPanel::DrawNamePanel(nLabel.c_str(), name, vLabel.c_str(), bVisible, rowWidth);
			glm::vec3 old = transComp.Transform.Position;
			UiPanel::DrawTransformPanel(transComp.Transform);			
			std::string attrName = "Attrbute##" + name;
			if (ImGui::CollapsingHeader(attrName.c_str(), ImGuiUtils::Flag_Collapse_Header))
			{
				std::string shadow = "CastShadow##" + name;
				ImGui::Checkbox(shadow.c_str(), &lightComp.CastShadow);
				std::string intensity = "Intensity##" + name;
				ImGui::DragFloat(intensity.c_str(), &lightComp.Intensity, 0.01f, 0.f, 10.f, "%.2f");
				std::string radius = "Radius##Lighting_" + name;
				ImGui::DragFloat(radius.c_str(), &lightComp.Radius, 0.01f, 0.f, 10.f, "%.2f");
				std::string falloff = "Falloff##" + name;
				ImGui::DragFloat(falloff.c_str(), &lightComp.Falloff, 0.01f, 0.f, 5.f, "%.2f");
				std::string radiance = "Radiance##" + name;
				ImGui::DragFloat3(radiance.c_str(), glm::value_ptr(lightComp.Radiance), 0.1f, 0.f, 1000.f, "%.1f");
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
				const float rowWidth = m_layoutParam.Viewport.Width;
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
			const float rowWidth = m_layoutParam.Viewport.Width;
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

	void EditorLayer::onDrawCameraPanel(Ref<Entity>& ent)
	{
		if (!ent || !ent->hasComponent<CameraComponent>())
			return;

		auto& comp = ent->getComponent<CameraComponent>();
		const float rowWidth = m_layoutParam.Viewport.Width;
		bool visible{ true };// always be visible

		UiPanel::DrawNamePanel("##camera_name", ent->getName(), "##camera_visibility", visible, rowWidth);

		auto attr = UiPanel::DrawTransformPanel(comp.Camera.transform());
		if (attr.test(DataAttrBits_Pos))
			comp.Camera.attrChange(CameraAttrBits_Pos);
		if (attr.test(DataAttrBits_Rot))
			comp.Camera.attrChange(CameraAttrBits_Rot);

		UiPanel::DrawCameraPanel(comp);
	}
}