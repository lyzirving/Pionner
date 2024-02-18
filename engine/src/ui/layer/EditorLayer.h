#ifndef __PIONNER_UI_LAYER_EDITOR_LAYER_H__
#define __PIONNER_UI_LAYER_EDITOR_LAYER_H__

#include "Layer.h"
#include "core/math/Transform.h"
#include "core/math/SphereCoord.h"

namespace pio
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const LayoutParams &param);
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const Timestep &ts) override;
		virtual void onWindowSizeChange(uint32_t width, uint32_t height) override;

	private:
		struct UiValue
		{			
			bool MeshSrcVisible{ true };
			SphereCoord DistantLightDir{};
			glm::vec4 MeshRot{ 1.f, 0.f, 0.f, 0.f };//wxyz
		};

	private:
		void onDrawScenePanel(Ref<Entity> &ent);
		void onDrawDistantLightPanel(Ref<Entity> &ent);
		void onDrawMeshSourcePanel(Ref<Entity> &ent);
		void onDrawMeshPanel(Ref<Entity> &ent);

	private:
		Ref<Entity> m_mainCameraEnt;
		Ref<Entity> m_sceneEnt;
		Ref<Entity> m_selectedEnt;
		UiValue m_UiVal;
	};
}

#endif