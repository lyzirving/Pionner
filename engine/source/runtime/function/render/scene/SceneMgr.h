#ifndef __RENDER_SCENE_MGR_H__
#define __RENDER_SCENE_MGR_H__

#include "RenderScene.h"
#include "Camera.h"
#include "Frustum.h"

#include "light/Light.h"

namespace Pionner
{
	class  WindowSystem;
	class  Rhi;
	class  LightComp;
	struct RenderParam;

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

		void swap(const LightComp &comp, const RenderParam &param);
		void shutdown();

		void resize(int width, int height);

	public:
		std::shared_ptr<RenderScene> m_scene;
		std::shared_ptr<Camera>      m_camera;
		std::shared_ptr<Frustum>     m_frustum;
		std::shared_ptr<Light>       m_lights[LIGNT_TYPE_COUNT];
		LightType                    m_curLight;
	};
}

#endif