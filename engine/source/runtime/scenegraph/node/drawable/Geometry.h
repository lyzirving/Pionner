#ifndef __SCENE_GRAPH_DRAWABLE_GEOMETRY_H__
#define __SCENE_GRAPH_DRAWABLE_GEOMETRY_H__

#include <vector>

#include "Drawable.h"
#include "gfx/matl/Material.h"

namespace pio
{
	namespace gfx
	{
		class Texture;
	}

	namespace sgf
	{
		class Geometry : public Drawable
		{
		public:
			Geometry();
			Geometry(const std::string &name);
			virtual ~Geometry();

			void addTexture(const std::shared_ptr<gfx::Texture> &texture);
			void setVertexArray(const std::vector<Vertex> &vertexArray);
			void setIndiceArray(const std::vector<uint32_t> &indiceArray);
			void setMaterial(const gfx::Material &material);

		protected:
			virtual void drawImplementation(RenderInfo &info) override;
			virtual bool upload() override;

			void renderMaterialDisplay(RenderInfo &info);

		protected:
			std::vector<Vertex>                        m_vertexs{};
			std::vector<uint32_t>                      m_indices{};
			std::vector<std::shared_ptr<gfx::Texture>> m_textures{};
			gfx::Material                              m_material{};
		};

		template <>
		bool Node::is<Geometry>();

		template <>
		Geometry *Node::as<Geometry>();
	}
}

#endif	