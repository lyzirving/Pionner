#ifndef __SCENE_GRAPH_DRAWABLE_H__
#define __SCENE_GRAPH_DRAWABLE_H__

#include "scenegraph/node/Node.h"

namespace pio
{
	namespace gfx
	{
		class Buffer;
	}

	namespace sgf
	{
		class Drawable : public Node
		{
		public:
			Drawable();
			Drawable(const std::string &name);
			virtual ~Drawable();

			virtual void draw(RenderInfo &info);

		protected:
			virtual void drawImplementation(RenderInfo &info) = 0;
			virtual bool upload() = 0;

		protected:
			std::shared_ptr<gfx::Buffer> m_vertexBuffer{ nullptr };
			std::shared_ptr<gfx::Buffer> m_indiceBuffer{ nullptr };
		};

		template <>
		bool Node::is<Drawable>();

		template <>
		Drawable *Node::as<Drawable>();
	}
}



#endif	