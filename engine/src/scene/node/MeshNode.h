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
		PIO_DEFINE_NODE_TYPE(NodeType::Mesh)
	public:
		MeshNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name);
		virtual ~MeshNode();

		virtual MeshType meshType() const = 0;
		virtual void update() override;
	};

	class PlaneNode : public MeshNode
	{
		PIO_DEFINE_MESH_TYPE(MeshType::Plane)
	public:
		PlaneNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name);
		~PlaneNode();		
	};

	class CubeNode : public MeshNode
	{
		PIO_DEFINE_MESH_TYPE(MeshType::Cube)
	public:
		CubeNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name);
		~CubeNode();
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