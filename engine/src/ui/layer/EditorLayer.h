#ifndef __PIONNER_UI_LAYER_EDITOR_LAYER_H__
#define __PIONNER_UI_LAYER_EDITOR_LAYER_H__

#include "Layer.h"
#include "core/math/Transform.h"

namespace pio
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const WindowLayoutParams &param);
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const Timestep &ts) override;
		virtual void onWindowSizeChange(uint32_t width, uint32_t height) override;

	private:
		struct UiQuat
		{
			float w{ 1.f };
			float x{ 0.f };
			float y{ 0.f };
			float z{ 0.f };

			UiQuat() {}
			UiQuat(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

			float *value_ptr() { return &w; }
		};

		struct UiValue
		{
			bool MeshSrcVisible{ true };
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