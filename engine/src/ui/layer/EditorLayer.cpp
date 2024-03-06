#include "EditorLayer.h"

#include "Application.h"

#include "asset/AssetsManager.h"

#include "scene/Components.h"

#include "animation/Animation.h"
#include "animation/Animator.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/struct/Mesh.h"
#include "gfx/debug/GDebugger.h"

#include "ui/ImGuiUtils.h"

#include "core/math/SphereCoord.h"

#include <imgui.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EditorLayer"

namespace pio
{
	static void DealSelectionChange(Ref<Entity> &cur, Ref<Entity> &old)
	{
		if (cur && cur->getNodeType() == NodeType::Mesh)
		{
			cur->setSelection(true);
		} 

		if (old && old->getNodeType() == NodeType::Mesh)
		{
			old->setSelection(false);
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

	void EditorLayer::onUpdate(const Timestep &ts)
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
				switch (select->getNodeType())
				{
					case NodeType::Scene:
						onDrawScenePanel(select);
						break;
					case NodeType::DistantLight:
						onDrawDistantLightPanel(select);
						break;
					case NodeType::MeshSource:
						onDrawMeshSourcePanel(select);
						break;
					case NodeType::Mesh:
						onDrawMeshPanel(select);
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
		if (ent && ent->hasComponent<SceneComponent>() && ent->hasComponent<RelationshipComponent>())
		{
			auto &sceneComp = ent->getComponent<SceneComponent>();
			auto &rlComp = ent->getComponent<RelationshipComponent>();
			const float rowWidth = m_layoutParam.Viewport.Width;
			
			ImGui::PushItemWidth(rowWidth);
			ImGui::InputText("##scene_name", const_cast<char *>(rlComp.Tag.data()),
							 rlComp.Tag.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			if (ImGui::CollapsingHeader("Physics##Scene", ImGuiUtils::Flag_Collapse_Header))
			{
				ImGui::Checkbox("Simulate##Scene_Physics", &sceneComp.Simulate);
			}
		}
	}

	void EditorLayer::onDrawDistantLightPanel(Ref<Entity> &ent)
	{
		if (ent && ent->hasComponent<DirectionalLightComponent>())
		{
			auto &lightComp = ent->getComponent<DirectionalLightComponent>();
			auto &rlComp = ent->getComponent<RelationshipComponent>();
			auto &transComp = ent->getComponent<TransformComponent>();

			const float rowWidth = m_layoutParam.Viewport.Width;
			ImGui::PushItemWidth(rowWidth);
			ImGui::InputText("##MainLight_Name", const_cast<char *>(rlComp.Tag.data()),
							 rlComp.Tag.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			if (ImGui::CollapsingHeader("DistantLight##Scene", ImGuiUtils::Flag_Collapse_Header))
			{						
				ImGui::Checkbox("CastShadow##DistantLight", &lightComp.CastShadow);
				ImGui::Combo("PCF", &lightComp.SdMode, ShadowModeNames, ShadowMode_Num, 3);
				ImGui::DragFloat("Bias##DistantLight", &lightComp.Bias, 0.00001f, 0.0001f, 0.1f, "%.5f");

				ImGui::DragFloat3("Position##DirectionalLight", glm::value_ptr(transComp.Transform.Position), 0.1f, -1000.f, 1000.f, "%.1f");
				
				glm::vec3 angle = transComp.Transform.Euler.angle();
				ImGui::DragFloat3("Rotation##DirectionalLight", &angle.x, 0.005f, -360.f, 360.f, "%.2f");
				transComp.Transform.Euler = angle;

				ImGui::DragFloat3("Radiance##DirectionalLight", glm::value_ptr(lightComp.Radiance), 0.1f, 0.f, 1000.f, "%.1f");			
				ImGui::SliderFloat("Intensity##DirectionalLight", &lightComp.Intensity, 0.01f, 2.f, "%.2f", 0);				
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
				
				// Visibility and Name
				{
					float cbWidth = 0.2f * rowWidth;
					float inTextWidth = 0.8f * rowWidth;
					// Visibility checkbox
					ImGui::PushItemWidth(cbWidth);
					ImGui::Checkbox("##meshSource_visibility", &this->m_UiVal.MeshSrcVisible);
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushItemWidth(inTextWidth);
					ImGui::InputText("##meshSource_name", const_cast<char *>(meshSrc->getName().c_str()),
									 meshSrc->getName().size(), ImGuiInputTextFlags_ReadOnly);
					ImGui::PopItemWidth();

					if (this->m_UiVal.MeshSrcVisible != meshSrcComp.Visible)
					{
						meshSrcComp.Visible = this->m_UiVal.MeshSrcVisible;
						const std::vector<Submesh> &submeshes = meshSrc->getSubmeshes();
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
				}

				// Global Pose
				{									
					if (ImGui::CollapsingHeader("GlobalPose##MeshSource", ImGuiUtils::Flag_Collapse_Header))
					{					
						glm::vec3 angle = meshSrc->GlobalPose.Euler.angle();						
						ImGui::DragFloat3("Position##meshSource", glm::value_ptr(meshSrc->GlobalPose.Position), 0.05f, -100.f, 100.f, "%.1f");
						ImGui::DragFloat3("Rotation##meshSource", &angle.x, 0.1f, -360.f, 360.f, "%.1f");
						ImGui::DragFloat3("Scale##meshSource", glm::value_ptr(meshSrc->GlobalPose.Scale), 0.1f, 0.f, 10.f, "%.1f");							
						meshSrc->GlobalPose.Euler = angle;
					}
				}

				//Animation
				{
					if (ent->hasComponent<AnimationComponent>() && meshSrc->hasAnimation())
					{
						auto &comp = ent->getComponent<AnimationComponent>();
						const std::vector<Ref<Animation>> &animations = meshSrc->getAnimation();

						uint32_t animCnt = animations.size();
						std::vector<const char *> aniNames;
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
	}

	void EditorLayer::onDrawMeshPanel(Ref<Entity> &ent)
	{
		if (ent && ent->hasComponent<MeshComponent>())
		{
			auto &meshComp = ent->getComponent<MeshComponent>();
			Ref<MeshBase> meshBase = AssetsManager::GetRuntimeAsset<MeshBase>(meshComp.Handle);
			if (meshBase)
			{
				const Submesh &submesh = meshBase->getMeshSource()->getSubmeshes()[meshComp.SubmeshIndex];	
				const float rowWidth = m_layoutParam.Viewport.Width;
				float cbWidth = 0.2f * rowWidth;
				float inTextWidth = 0.8f * rowWidth;
				// Visibility checkbox
				ImGui::PushItemWidth(cbWidth);
				ImGui::Checkbox("##mesh_visibility", &meshComp.Visible);
				ImGui::PopItemWidth();
				
				ImGui::SameLine();
				// Mesh name
				ImGui::PushItemWidth(inTextWidth);
				ImGui::InputText("##submesh_name", const_cast<char *>(submesh.MeshName.c_str()), 
								 submesh.MeshName.size(), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();

				if (ent->hasComponent<TransformComponent>())
				{			
					TransformComponent &comp = ent->getComponent<TransformComponent>();					
					if (ImGui::CollapsingHeader("Transform##Mesh", ImGuiUtils::Flag_Collapse_Header))
					{						
						glm::vec3 angle = comp.Transform.Euler.angle();						
						ImGui::DragFloat3("Position##mesh", glm::value_ptr(comp.Transform.Position), 0.05f, -100.f, 100.f, "%.1f");
						ImGui::DragFloat3("Rotation##mesh", &angle.x, 0.1f, -360.f, 360.f, "%.1f");
						ImGui::DragFloat3("Scale##mesh", glm::value_ptr(comp.Transform.Scale), 0.1f, 0.f, 10.f, "%.1f");	
						comp.Transform.Euler = angle;
					}
				}
			}
		}
	}
}