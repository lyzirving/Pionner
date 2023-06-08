#ifndef __RENDER_VISUAL_ANGLE_VIEW_H__
#define __RENDER_VISUAL_ANGLE_VIEW_H__

#include <vector>

#include "function/ui/WindowView.h"

namespace Pionner
{
	class  MeshComp;
	class  CoordinateAxis;
	class  Sphere;
	struct Vertex;

	class VisualAngleView : public WindowView
	{
	public:
		VisualAngleView();
		virtual ~VisualAngleView();

		virtual void draw(RenderParam &param) override;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) override;

	private:
		void buildGridData();

	private:
		std::shared_ptr<MeshComp>       m_meshComp;
		std::shared_ptr<CoordinateAxis> m_coordinateAxis;
		std::shared_ptr<Sphere>         m_sphere;
		std::vector<Vertex>             m_vertexArray;
		std::vector<uint32_t>           m_indice;
		ViewLayout                      m_renderport;
	};
}

#endif