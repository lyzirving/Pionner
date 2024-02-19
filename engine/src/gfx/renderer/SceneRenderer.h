#ifndef __PIONNER_GFX_RENDERER_SCENE_RENDERER_H__
#define __PIONNER_GFX_RENDERER_SCENE_RENDERER_H__

#include "RenderData.h"
#include "gfx/struct/Camera.h"

namespace pio
{
	class Scene;
	class MeshBase;
	class Mesh;
	class StaticMesh;
	class Transform;
	class UniformBufferSet;
	class RenderPass;
	class Texture2D;

	/*
	* SceneRenderer is somewhere used to store rendering data and execute render command.
	*/
	class SceneRenderer
	{		
	public:
		SceneRenderer();
		virtual ~SceneRenderer();

		virtual void onAttach(const Scene &scene);
		virtual void onDetach(const Scene &scene);
		// Ensure to be called in render thread
		virtual void onRenderDestroy(const Scene &scene);

		virtual void beginScene(const Scene &scene, const Camera &camera);
		virtual void endScene(const Scene &scene);

		virtual void submitMesh(Ref<MeshBase> &mesh, uint32_t submeshIndex, Transform &transform, const RenderState &state);
		// Delayed mesh is drawn in the way of forward rendering at the final pass
		virtual void submitDelayedMesh(Ref<MeshBase> &mesh, uint32_t submeshIndex, Transform &transform, const RenderState &state);

	public:
		inline Ref<Texture2D> getCompositeTexture() { return m_compositeTexture; }

	private:				
		void createShadowPass(uint32_t w, uint32_t h);
		void createForwardPass(uint32_t w, uint32_t h);
		void createDeferredPass(uint32_t w, uint32_t h);
		void createGBufferPass(uint32_t w, uint32_t h);
		void createLightPass(uint32_t w, uint32_t h);

		void flushDrawList(const Scene &scene);

		void shadowPass(const Scene &scene);

		// --------- forward pass -----------
		void forwardRendering(const Scene &scene);
		void geometryPass(const Scene &scene);
		// ----------------------------------

		// --------- deferred pass ----------
		void deferredRendering(const Scene &scene);
		void geometryPass_deferred(const Scene &scene);
		void lightingPass_deferred(const Scene &scene);
		// ----------------------------------

	private:
		bool m_active{ false };
		std::map<MeshKey, DrawCommand> m_meshDraws{};
		std::map<MeshKey, DrawCommand> m_delayedMeshDraws{};
		std::map<MeshKey, DrawCommand> m_shadowPassDraws{};

		CameraUD m_cameraUD{};
		Ref<UniformBufferSet> m_uniformBuffers{};
		
		Ref<RenderPass> m_distantLightShadowPass;
		Ref<RenderPass> m_pointLightShadowPass;

		Ref<Texture2D> m_compositeTexture;
		Ref<RenderPass> m_forwardPass;
		// -------------- deferred shading -----------------
		Ref<RenderPass> m_GBufferPass;
		Ref<RenderPass> m_lightPass;
		// -------------------------------------------------
	};
}

#endif