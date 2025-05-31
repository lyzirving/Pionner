#pragma once

#include "ui/imgui/ImGuiWindow.h"
#include "event/EventSocket.h"

namespace pio
{
	class CameraNode;
	class DirectionalLitNode;
	class PointLitNode;
	class StaticMeshNode;
	class SkinnedMeshNode;
	class MaterialBall;

	class TransformComponent;
	class StaticMeshComponent;
	class SkeletalMeshComponent;
	class StaticMesh;
	class SkinnedMesh;
	class Material;
	class StandardMaterial;
	class TexturedMaterial;

	class DetailPanel : public EventSocket, public ImGuiWindow
	{
		RTTR_ENABLE(ImGuiWindow)
	public:
		DetailPanel(const std::string& name = "Detail");
		~DetailPanel() = default;
		virtual void OnAttach(const Ref<Scene>& scene) override;
		virtual void OnDetach() override;

		virtual void OnDraw(const Ref<RenderContext>& context) override; 

	private:
		void OnNodeSelect(const Ref<Event>& event);

		void ShowCameraDetail(CameraNode* node);
		void ShowStaticMeshDetail(StaticMeshNode* node);
		void ShowSkinnedMeshDetail(SkinnedMeshNode* node);
		void ShowDirectionalLitDetail(DirectionalLitNode* node);
		void ShowPointLitDetail(PointLitNode* node);

		void DrawTransformCompDetail(const Ref<TransformComponent>& comp);
		void DrawStaticMeshCompDetail(const Ref<StaticMeshComponent>& comp);
		void DrawStaticMeshDetail(const Ref<StaticMesh>& mesh);
		void DrawSkeletalMeshCompDetail(const Ref<SkeletalMeshComponent>& comp);
		void DrawSkinnedMeshDetail(const Ref<SkinnedMesh>& mesh);
		void DrawSkeletalAnimationDetail(const Ref<SkeletalMeshComponent>& comp);
		void DrawMaterialDetail(const std::vector<Ref<Material>>& materials);
		void DrawMaterialElem(const Ref<Material>& elem);
		void DrawMaterialBall(const Ref<Material>& elem);
		void DrawStandardMaterial(StandardMaterial* material);

		void OpenImageModal(bool bOpen, const char* title, int32_t texId);

	private:				
		std::map<std::string, bool> m_ModalOpenStat;
		Ref<MaterialBall> m_MaterialBall;
		std::atomic_uint32_t m_Select{ InvalidId };
	};
}