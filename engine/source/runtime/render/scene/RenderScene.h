#ifndef __RENDER_SCENE_H__
#define __RENDER_SCENE_H__

#include <memory>
#include <vector>

#include "render/scene/RenderSceneDef.h"

namespace pio
{
	class  Rhi;
	class  RenderLayer;
	class  Camera;
	class  Frustum;
	struct RenderParam;

	class RenderScene
	{
	public:
		RenderScene();
		~RenderScene();

		void initialize(const std::shared_ptr<Rhi> &rhi);
		void shutdown();

		void forwardRender(RenderParam &param);

	private:
		static std::shared_ptr<RenderLayer> createLayer(SceneLayerType type);

	private:
		std::shared_ptr<Rhi>         m_rhi;
		std::shared_ptr<RenderLayer> m_layers[LAYER_COUNT];
	};
}

#endif