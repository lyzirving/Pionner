#include "ObjectLayer.h"

#include "scenegraph/visitor/DrawVisitor.h"

#include "core/system/TimeUtil.h"
#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ObjectLayer"

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
			if (!m_root)
				return;

			DrawVisitor visitor{};
			visitor.setRenderInfo(&info);
			m_root->accept(&visitor);
		}
	}
}