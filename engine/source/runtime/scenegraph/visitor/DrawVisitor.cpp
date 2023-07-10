#include "DrawVisitor.h"

#include "scenegraph/node/drawable/Drawable.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DrawVisitor"

namespace pio
{
	namespace sgf
	{
		DrawVisitor::DrawVisitor() : NodeVisitor(TRAVERSE_ALL_CHILDREN)
		{
		}

		DrawVisitor::~DrawVisitor()
		{
			m_info = nullptr;
		}

		void DrawVisitor::apply(Node *node)
		{
			if (!node || !m_info)
			{
				LOG_ERR("invalid state");
				return;
			}
			if (node->isLeaf())
			{
				if (node->is<Drawable>())
				{
					auto *p = node->as<Drawable>();
					if (p)
					{
						p->draw(*m_info);
					}
				}
			}
			else
			{
				node->descend(this);
			}
		}
	}
}