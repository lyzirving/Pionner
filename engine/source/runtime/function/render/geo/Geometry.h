#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include <string>
#include <memory>

namespace Pionner
{
	struct RenderParam;
	class  MeshComp;
	class  TransformComp;

	class Geometry
	{
	public:
		Geometry(const char *name);
		~Geometry();

		virtual void draw(RenderParam &param) = 0;
		virtual void initialize(RenderParam &param) = 0;

		inline const std::string &name() { return m_name; }
		inline const std::shared_ptr<MeshComp> &getMeshComp() { return m_mesh; }
		inline const std::shared_ptr<TransformComp> &getTransformComp() { return m_transform; }

	protected:
		std::string m_name;
		std::shared_ptr<MeshComp>      m_mesh;
		std::shared_ptr<TransformComp> m_transform;
	};
}

#endif