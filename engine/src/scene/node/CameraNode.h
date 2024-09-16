#ifndef __PIONNER_SCENE_NODE_CAMERA_NODE_H__
#define __PIONNER_SCENE_NODE_CAMERA_NODE_H__

#include "Node.h"

namespace pio
{
	class UniformBuffer;
	class Camera;
	class RenderTarget;
	struct CameraUD;

	class CameraNode : public Node
	{
		PIO_DEFINE_NODE_TYPE(NodeType::Camera)
	public:
		CameraNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name);
		~CameraNode();

		virtual void update(Ref<RenderContext>& context) override;

		void setDepth(int32_t depth) { m_depth = depth; }
		void setRenderTarget(const Ref<RenderTarget>& target) { m_target = target; }

		Ref<Camera>& camera() { return m_camera; }
		Ref<RenderTarget>& renderTarget() { return m_target; }

		int32_t depth() const { return m_depth; }
		const Ref<Camera>& camera() const { return m_camera; }
		const Ref<RenderTarget>& renderTarget() const { return m_target; }

	private:
		int32_t m_depth{ 0 };

		Ref<Camera> m_camera;
		Ref<RenderTarget> m_target;
		Ref<CameraUD> m_UData;
		Ref<UniformBuffer> m_UBuffer;		

	private:
		friend class Node;
	};

	template<>
	bool Node::is<CameraNode>() const;

	template<>
	Ref<UniformBuffer> Node::getRenderingData<CameraNode>() const;
}

#endif