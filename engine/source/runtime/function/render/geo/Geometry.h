#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include <string>
#include <memory>

#include "function/framework/comp/TransformComp.h"
#include "function/framework/comp/MeshComp.h"

#include "GeoDef.h"

namespace Pionner
{
	struct RenderParam;
	class  Shader;

	class Geometry
	{
	public:
		static std::shared_ptr<Geometry> createGeometry(GeometryType type);

	public:
		Geometry(const char *name);
		virtual ~Geometry();

		virtual void draw(RenderParam &param) = 0;
		virtual void initialize(RenderParam &param) = 0;
		virtual bool dealShader(RenderParam &param, std::shared_ptr<Shader> &shader) = 0;
		virtual bool dealDepthShader(RenderParam &param, /*out*/std::shared_ptr<Shader> &shader);
		virtual bool isInitialized();

		inline const std::string &name() { return m_name; }
		inline const std::shared_ptr<MeshComp> &getMeshComp() { return m_mesh; }
		inline const std::shared_ptr<TransformComp> &getTransformComp() { return m_transform; }

		void translate(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);

		template<class T>
		bool is();

		template<class T>
		T *as();

	private:
		bool dealDirectionLightShader(RenderParam &param, /*out*/std::shared_ptr<Shader> &shader);
		bool dealPointLightShader(RenderParam &param, /*out*/std::shared_ptr<Shader> &shader);

	protected:
		GeometryType m_type;
		std::string  m_name;
		std::shared_ptr<MeshComp>      m_mesh;
		std::shared_ptr<TransformComp> m_transform;
	};
}

#endif