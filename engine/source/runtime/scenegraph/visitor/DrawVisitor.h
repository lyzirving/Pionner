#ifndef __SCENE_GRAPH_NODE_DRAW_VISITOR_H__
#define __SCENE_GRAPH_NODE_DRAW_VISITOR_H__

#include "NodeVisitor.h"

#include "scenegraph/SceneGrfDef.h"

namespace pio
{
	namespace sgf
	{
		class DrawVisitor : public NodeVisitor
		{
		public:
			DrawVisitor();
			virtual ~DrawVisitor();

			virtual void apply(Node *node) override;

			inline void setRenderInfo(RenderInfo *info) { m_info = info; }

		private:
			RenderInfo *m_info{ nullptr };
		};
	}
}

#endif