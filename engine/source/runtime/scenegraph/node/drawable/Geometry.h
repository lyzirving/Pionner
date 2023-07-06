#ifndef __SCENE_GRAPH_DRAWABLE_GEOMETRY_H__
#define __SCENE_GRAPH_DRAWABLE_GEOMETRY_H__

#include <vector>

#include "Drawable.h"
#include "matl/Material.h"

namespace pio
{
	namespace sgf
	{
		class Geometry : public Drawable
		{
		public:
			Geometry();
			Geometry(const std::string &name);
			virtual ~Geometry();

			void setVertexArray(const std::vector<Vertex> &vertexArray);
			void setIndiceArray(const std::vector<uint32_t> &indiceArray);

		protected:
			virtual void drawImplementation(RenderInfo &info) override;

		protected:
			std::vector<Vertex>   m_vertexs;
			std::vector<uint32_t> m_indices;
			matl::Material   m_material;
		};

		template <>
		bool Node::is<Geometry>() const;

		template <>
		Geometry *Node::as<Geometry>();
	}
}

#endif	