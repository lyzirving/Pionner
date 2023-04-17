#ifndef __RENDER_SCENE_H__
#define __RENDER_SCENE_H__

#include <memory>

#include "function/render/scene/RenderSceneDef.h"

namespace Pionner
{
	class Rhi;
	class RenderLayer;

	class RenderScene
	{
	public:
		RenderScene();
		~RenderScene();

		void initialize(const std::shared_ptr<Rhi> &rhi);
		void shutdown();

		void forwardRender();

	private:
		static std::shared_ptr<RenderLayer> createLayer(SceneLayerType type, const std::shared_ptr<Rhi> &rhi);

	private:
		std::shared_ptr<Rhi>         m_rhi;
		std::shared_ptr<RenderLayer> m_layers[LAYER_COUNT];
	};
}

#endif