#ifndef __PIONNER_SCENE_NODE_MESH_NODE_H__
#define __PIONNER_SCENE_NODE_MESH_NODE_H__

#include "Node.h"
#include "gfx/resource/RenderingData.h"

namespace pio
{
	#define PIO_DEFINE_MESH_TYPE(TypeName)  public:\
									        static  MeshType StaticMeshType() { return TypeName; }\
									        virtual MeshType meshType() const override { return StaticMeshType(); }

	class MeshNode : public Node
	{
		PIO_NODE_DECLARE(MeshNode, NodeType::Mesh)
	public:
		virtual ~MeshNode();

		virtual MeshType meshType() const = 0;
		virtual void update(Ref<RenderContext>& context) override;
		virtual void onInit() override;
	};

	class PlaneNode : public MeshNode
	{
		PIO_NODE_DECLARE_CONSTRUCTOR(PlaneNode)
		PIO_DEFINE_MESH_TYPE(MeshType::Plane)
	public:
		~PlaneNode();

		virtual void onInit() override;
	};

	class CubeNode : public MeshNode
	{
		PIO_NODE_DECLARE_CONSTRUCTOR(CubeNode)
		PIO_DEFINE_MESH_TYPE(MeshType::Cube)
	public:
		~CubeNode();

		virtual void onInit() override;
	};

	template<>
	bool Node::is<MeshNode>() const;

	template<>
	bool Node::is<PlaneNode>() const;

	template<>
	bool Node::is<CubeNode>() const;

	template<>
	MeshRenderingItem Node::getRenderingData<MeshNode>() const;
}

#endif