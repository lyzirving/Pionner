#ifndef __RENDER_GEO_OBJ_H__
#define __RENDER_GEO_OBJ_H__

#include <string>
#include <memory>

namespace Pionner
{
	struct RenderParam;
	class  MeshComp;

	class GeoObj
	{
	public:
		GeoObj(const char *name);
		~GeoObj();

		virtual void draw(RenderParam &param) = 0;
		virtual void initialize(RenderParam &param) = 0;

		inline const std::string &name() { return m_name; }

	protected:
		std::string m_name;
		std::shared_ptr<MeshComp> m_meshComp;
	};
}

#endif