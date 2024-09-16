#ifndef __PIONNER_SCENE_SCENE_H__
#define __PIONNER_SCENE_SCENE_H__

#include "asset/Asset.h"
#include "Registry.h"

namespace pio
{
	class Node;
	class RenderContext;
	class RenderPipeline;	
	class CameraNode;

	class Scene : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::SceneResource)
	public:
		Scene() : Asset() {}
		~Scene() = default;

		virtual void onAttach();
		virtual void onDetach();

		virtual void onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline, std::vector<Ref<CameraNode>>& camNodes);

		template<typename T>
		Ref<Node> addNode(Ref<RenderContext>& context, const std::string& name = "")
		{
			auto node = m_registry.create<T>(context, name);
			m_nodes.push_back(node);
			return node;
		}

		void setCameraNode(const Ref<CameraNode>& cam) { if (m_cameraNode != cam)  { m_cameraNode = cam; } }

		void removeNode(Ref<Node>& node);
		void removeAllNodes();

		Registry& registry() { return m_registry; }

		const Registry& registry() const { return m_registry; }
		const Ref<Node>& getNode(uint32_t index) const { return m_registry.getNode(index); }
		const std::vector<Ref<Node>>& nodes() const { return m_nodes; }
		const Ref<CameraNode>& cameraNode() const { return m_cameraNode; }

	private:
		Registry m_registry{};

		Ref<CameraNode> m_cameraNode;
		std::vector<Ref<Node>> m_nodes;
	};

	template<>
	bool Asset::is<Scene>() const;
}

#endif