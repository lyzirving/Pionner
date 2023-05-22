#ifndef __RENDER_SCENE_MGR_H__
#define __RENDER_SCENE_MGR_H__

#include "function/render/scene/RenderScene.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"
#include "function/render/scene/Layout.h"

namespace Pionner
{
	class  WindowSystem;
	class  Rhi;

	struct SceneMgrInitInfo
	{
		std::shared_ptr<WindowSystem> window;
		std::shared_ptr<Rhi>          rhi;
	};

	class SceneMgr
	{
	public:
		SceneMgr();
		~SceneMgr();

		void initialize(SceneMgrInitInfo &info);
		void shutdown();

		void reviseLayout(int width, int height);

	public:
		std::shared_ptr<RenderScene>  m_scene;
		std::shared_ptr<Layout>       m_layout;
		std::shared_ptr<Camera>       m_camera;
		std::shared_ptr<Frustum>      m_frustum;
	};
}

#endif