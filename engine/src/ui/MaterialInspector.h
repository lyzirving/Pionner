#ifndef __PIONNER_UI_MATERIAL_INSPECTOR_H__
#define __PIONNER_UI_MATERIAL_INSPECTOR_H__

#include "UiDef.h"

namespace pio
{
	class MeshRenderBuffer;
	class Material;
	class RenderContext;
	class UniformBuffer;
	class Camera;
	class FrameBuffer;
	class Shader;
	class Texture;
	struct CameraUD;
	struct DirectionalLightUD;

	class MaterialInspector
	{		
	public:
		static MaterialInspector* Get() { return s_instance; }
		static void Init(const Ref<RenderContext>& context);
		static void Shutdown();
	private:
		MaterialInspector(const Ref<RenderContext>& ctx);
		~MaterialInspector() = default;
	private:
		static MaterialInspector* s_instance;
		static std::mutex s_mutex;

	public:
		float aspect() const { return m_w / m_h; }
		void setMaterial(const Ref<Material>& mat) { m_material = mat; }
		void setVisible(bool val) { m_visible = val; }
		bool bVisible() const { return m_visible; }

		Ref<Texture> colorBuffer() const;
		void onDraw();

	private:
		bool m_visible{ false };
		float m_w{ 0.f }, m_h{ 0.f };
		Ref<RenderContext> m_context;
		Ref<Material> m_material;
		Ref<Camera> m_camera;

		Ref<DirectionalLightUD> m_litData;
		Ref<UniformBuffer> m_litBuff;

		Ref<CameraUD> m_camData;
		Ref<UniformBuffer> m_camBuff;

		Ref<MeshRenderBuffer> m_meshBuff;

		Ref<FrameBuffer> m_frameBuff;
		Ref<Shader> m_shader;
	};
}

#endif