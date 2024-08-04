#ifndef __PIONNER_UI_LAYER_SCENE_HIERARCHY_LAYER_H__
#define __PIONNER_UI_LAYER_SCENE_HIERARCHY_LAYER_H__

#include "Layer.h"

namespace pio
{
	class SceneHierarchyLayer : public Layer
	{
	public:
		SceneHierarchyLayer(const LayoutParams &param);
		virtual ~SceneHierarchyLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdateUI(const Timestep &ts) override;
		virtual void onWindowSizeChange(uint32_t width, uint32_t height) override;

	private:
		uint32_t m_selectedNode{ NullIndex };
	};
}

#endif