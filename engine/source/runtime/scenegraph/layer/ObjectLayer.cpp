#include "ObjectLayer.h"

#include "render/rhi/RhiHeader.h"

namespace pio
{
	namespace sgf
	{
		ObjectLayer::ObjectLayer() : Layer(LAYER_TYPE_OBJ)
		{
			m_root = std::make_shared<Group>(ROOT_NODE);
		}

		ObjectLayer::~ObjectLayer() = default;

		void ObjectLayer::update(RenderInfo &info)
		{
		}
	}
}