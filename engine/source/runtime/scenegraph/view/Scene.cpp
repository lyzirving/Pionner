#include <algorithm>

#include "Scene.h"
#include "camera/MainCamera.h"

#include "scenegraph/view/layer/ObjectLayer.h"

namespace pio
{
	namespace sgf
	{
		Scene::Scene()
		{
			createCameras();
			createLayers();
		}

		Scene::~Scene() = default;

		void Scene::addNode(std::shared_ptr<Node> &node)
		{
			if ((m_layers.size() > LAYER_TYPE_OBJ) && m_layers[LAYER_TYPE_OBJ])
			{
				m_layers[LAYER_TYPE_OBJ]->addNode(node);
			}
		}

		void Scene::addNode(const std::string &parentNodeName, std::shared_ptr<Node> &node)
		{
			if ((m_layers.size() > LAYER_TYPE_OBJ) && m_layers[LAYER_TYPE_OBJ])
			{
				m_layers[LAYER_TYPE_OBJ]->addNode(parentNodeName, node);
			}
		}

		void Scene::release()
		{
			auto itrLayer = m_layers.begin();
			while (itrLayer != m_layers.end())
			{
				if ((*itrLayer).get())
				{
					(*itrLayer)->release();
					(*itrLayer).reset();
				}
				itrLayer = m_layers.erase(itrLayer);
			}

			auto itrCam = m_cameras.begin();
			while (itrCam != m_cameras.end())
			{
				if ((*itrCam).get())
				{
					(*itrCam)->release();
					(*itrCam).reset();
				}
				itrCam = m_cameras.erase(itrCam);
			}
		}

		void Scene::setWndSize(int32_t width, int32_t height)
		{
			// set each camera's render viewport
			for (auto &cam : m_cameras)
			{
				if (!cam)
					continue;
				if (cam->getCameraId() == CAM_ID_MAIN)
				{
					cam->setViewport(0, 0, width, height);
					cam->setAspect(float(width) / float(height));
				}
			}
		}

		void Scene::tick(uint64_t deltaMs)
		{
			RenderInfo info{};
			info.deltaMs = deltaMs;
			update(info);
		}

		bool Scene::layerSorter(const std::shared_ptr<Layer> &lhs, const std::shared_ptr<Layer> &rhs)
		{
			if (!lhs)
			{
				return false;
			}
			if (!rhs)
			{
				return true;
			}
			return lhs->getLayerType() <= rhs->getLayerType();
		}

		void Scene::createCameras()
		{
			auto mainCam = std::shared_ptr<Camera>(new MainCamera);
			mainCam->setVisible(true);
			m_cameras.push_back(mainCam);
		}

		void Scene::createLayers()
		{
			auto objLayer = std::shared_ptr<Layer>(new ObjectLayer);
			m_layers.push_back(objLayer);
		}

		void Scene::update(RenderInfo &info)
		{
			if (m_sortLayer)
			{
				std::sort(m_layers.begin(), m_layers.end(), Scene::layerSorter);
				m_sortLayer = false;
			}

			for (auto &cam : m_cameras)
			{
				if (cam && cam->isVisible())
				{
					cam->update(m_layers, info);
				}
			}
		}
	}
}