#ifndef __PIONNER_SCENE_NODE_MESH_NODE_H__
#define __PIONNER_SCENE_NODE_MESH_NODE_H__

#include "Node.h"
#include "MovableNode.h"
#include "gfx/resource/RenderingData.h"

namespace pio
{
	#define PIO_DEFINE_MESH_TYPE(TypeName)  private:\
									        static  MeshType StaticMeshType() { return TypeName; }\
											public:\
									        virtual MeshType meshType() const override { return StaticMeshType(); }
	class Material;

	class MeshNode : public MovableNode
	{
		PIO_NODE_DECLARE(MeshNode, NodeType::Mesh)
	public:
		MeshNode();
		virtual ~MeshNode() = default;

		virtual MeshType meshType() const = 0;	
		virtual void onInit() override;
		virtual void onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData) override;

		void setMaterial(const Ref<Material>& material);

		bool bDirty() const { return m_invalidate; }
		void invalidate(bool val = true) { m_invalidate = val; }

	protected:
		virtual void onUpdateDerived(Ref<RenderContext>& context, Ref<CameraNode>& camNode) {};

	protected:
		bool m_invalidate{ false };
	};

	class PlaneNode : public MeshNode
	{		
		PIO_DEFINE_MESH_TYPE(MeshType::Plane)
	public:		
		PlaneNode(float w = 5.f, float h = 5.f);
		~PlaneNode() = default;

		virtual void onInit() override;

		void setWidth(float w);
		void setHeight(float h);

		float getWidth() const { return m_w; }
		float getHeight() const { return m_h; }

	protected:
		virtual void onUpdateDerived(Ref<RenderContext>& context, Ref<CameraNode>& camNode) override;

	private:
		float m_w{ 10.f }, m_h{ 8.f };
	};

	class CubeNode : public MeshNode
	{		
		PIO_DEFINE_MESH_TYPE(MeshType::Cube)
	public:		
		CubeNode(float size = 2.f);
		~CubeNode() = default;

		virtual void onInit() override;

	private:
		float m_size{ 2.f };
	};

	class SphereNode : public MeshNode
	{		
		PIO_DEFINE_MESH_TYPE(MeshType::Sphere)
	public:
		SphereNode(float radius = 2.f, int32_t itr = 64);
		~SphereNode() = default;

		virtual void onInit() override;

	private:
		float m_radius{ 2.f };
		int32_t m_itr{ 64 };
	};

	template<>
	bool Node::is<MeshNode>() const;

	template<>
	bool Node::is<PlaneNode>() const;

	template<>
	bool Node::is<CubeNode>() const;

	template<>
	bool Node::is<SphereNode>() const;
}

#endif