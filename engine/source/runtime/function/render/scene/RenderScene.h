#ifndef __RENDER_SCENE_H__
#define __RENDER_SCENE_H__

#include <memory>
#include <vector>

#include "function/render/scene/RenderSceneDef.h"

namespace Pionner
{
	class  Rhi;
	class  RenderLayer;
	class  RenderEntity;
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

		void addEntities(const std::vector<std::shared_ptr<RenderEntity>> &entities);
		void forwardRender(RenderParam &param);

	private:
		static std::shared_ptr<RenderLayer> createLayer(SceneLayerType type, const std::shared_ptr<Rhi> &rhi);

	private:
		std::shared_ptr<Rhi>         m_rhi;
		std::shared_ptr<RenderLayer> m_layers[LAYER_COUNT];
	};
}

#endif