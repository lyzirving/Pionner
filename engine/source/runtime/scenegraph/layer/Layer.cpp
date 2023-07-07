#include "Layer.h"

namespace pio
{
	namespace sgf
	{
		Layer::Layer()
		{
		}

		Layer::Layer(LayerType type) : m_type(type)
		{
		}

		Layer::~Layer()
		{
			m_root.reset();
		}

		void Layer::addNode(std::shared_ptr<Node> &node)
		{
			if (m_root)
			{
				m_root->addChild(node);
			}
		}

		void Layer::addNode(const std::string &parentNodeName, std::shared_ptr<Node> &node)
		{
			if (m_root)
			{
				m_root->addChild(parentNodeName, node);
			}
		}
	}
}