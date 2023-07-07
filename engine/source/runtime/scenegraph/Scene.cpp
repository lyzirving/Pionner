#include "Scene.h"

#include "layer/ClearLayer.h"
#include "layer/ObjectLayer.h"

namespace pio
{
	namespace sgf
	{
		Scene::Scene()
		{
			createLayers();
		}

		Scene::~Scene() = default;

		void Scene::addNode(std::shared_ptr<Node> &node)
		{
			if (m_layers[LAYER_TYPE_OBJ])
			{
				m_layers[LAYER_TYPE_OBJ]->addNode(node);
			}
		}

		void Scene::addNode(const std::string &parentNodeName, std::shared_ptr<Node> &node)
		{
			if (m_layers[LAYER_TYPE_OBJ])
			{
				m_layers[LAYER_TYPE_OBJ]->addNode(parentNodeName, node);
			}
		}

		void Scene::update(RenderInfo &info)
		{
			for (auto &layer : m_layers)
			{
				if (layer)
					layer->update(info);
			}
		}

		void Scene::release()
		{
			for (auto &layer : m_layers)
			{
				if (layer) { layer->release(); }
				layer.reset();
			}
		}

		void Scene::createLayers()
		{
			m_layers[LAYER_TYPE_CLEAR] = std::shared_ptr<Layer>(new ClearLayer);
			m_layers[LAYER_TYPE_OBJ] = std::shared_ptr<Layer>(new ObjectLayer);
		}
	}
}