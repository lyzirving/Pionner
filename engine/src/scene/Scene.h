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
		Scene(const Ref<RenderContext>& ctx) : Asset(), m_context(ctx) {}
		~Scene() = default;

		virtual void onAttach();
		virtual void onDetach();

		virtual void onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline, std::vector<Ref<CameraNode>>& camNodes);

		template<typename T, typename ... Args>
		Ref<Node> addNode(const std::string& name, const Args&... args)
		{
			auto scene = self<Scene>();
			auto node = m_registry.create<T>(m_context, scene, name, args...);
			m_nodes.push_back(node);
			NotifyAttachScene(scene, node);
			return node;
		}

		template<typename T = Node>
		Ref<T> getNode(uint32_t index) const
		{
			return m_registry.getNode<T>(index);
		}

		void setCameraNode(const Ref<CameraNode>& cam) { if (m_cameraNode != cam)  { m_cameraNode = cam; } }

		void removeNode(Ref<Node>& node);
		void removeAllNodes();

		Registry& registry() { return m_registry; }

		const Registry& registry() const { return m_registry; }		
		const std::vector<Ref<Node>>& nodes() const { return m_nodes; }
		const Ref<CameraNode>& cameraNode() const { return m_cameraNode; }

	private:
		static void NotifyAttachScene(Ref<Scene>& scene, Ref<Node>& node);

	private:
		Registry m_registry{};

		Ref<RenderContext> m_context;
		Ref<CameraNode> m_cameraNode;
		std::vector<Ref<Node>> m_nodes;

	private:
		friend class Node;
	};

	template<>
	bool Asset::is<Scene>() const;
}

#endif