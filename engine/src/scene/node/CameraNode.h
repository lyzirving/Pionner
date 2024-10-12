#ifndef __PIONNER_SCENE_NODE_CAMERA_NODE_H__
#define __PIONNER_SCENE_NODE_CAMERA_NODE_H__

#include "MovableNode.h"

namespace pio
{
	class UniformBuffer;
	class Camera;
	class RenderTarget;
	struct CameraUD;

	class CameraNode : public MovableNode
	{
		PIO_NODE_DECLARE(CameraNode, NodeType::Camera)
	public:
		CameraNode();
		~CameraNode() = default;
		
		virtual void onInit() override;
		virtual void onUpdate(Ref<RenderContext>& context, RenderingData& renderingData) override;

		void setDepth(int32_t depth) { m_depth = depth; }
		void setRenderTarget(const Ref<RenderTarget>& target) { m_target = target; }

		Ref<Camera>& camera() { return m_camera; }
		Ref<RenderTarget>& renderTarget() { return m_target; }

		int32_t depth() const { return m_depth; }
		const Ref<Camera>& camera() const { return m_camera; }
		const Ref<RenderTarget>& renderTarget() const { return m_target; }

	protected:
		void onUpdateInner(Ref<RenderContext>& context);

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
}

#endif