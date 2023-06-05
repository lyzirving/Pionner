#ifndef __RENDER_VISUAL_ANGLE_VIEW_H__
#define __RENDER_VISUAL_ANGLE_VIEW_H__

#include <vector>

#include "function/ui/WindowView.h"

namespace Pionner
{
	class  MeshComp;
	struct Vertex;

	class VisualAngleView : public WindowView
	{
	public:
		VisualAngleView();
		virtual ~VisualAngleView();

		virtual void draw(RenderParam &param) override;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) override;

	private:
		void init();

	private:
		std::shared_ptr<MeshComp> m_meshComp;
		std::vector<Vertex>       m_vertexArray;
		std::vector<uint32_t>     m_indice;
		ViewLayout                m_renderport;
	};
}

#endif