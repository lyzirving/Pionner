#ifndef __PIONNER_UI_LAYER_EDITOR_LAYER_H__
#define __PIONNER_UI_LAYER_EDITOR_LAYER_H__

#include "Layer.h"
#include "core/math/Transform.h"

namespace pio
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const LayoutParams &param);
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdateUI(const Timestep &ts) override;
		virtual void onWindowSizeChange(uint32_t width, uint32_t height) override;

	private:
		void onDrawScenePanel(Ref<Entity> &ent);
		void onDrawMeshSourcePanel(Ref<Entity> &ent);
		void onDrawMeshPanel(Ref<Entity> &ent);

	private:		
		Ref<Entity> m_selectedEnt;
	};
}

#endif