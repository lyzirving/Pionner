#ifndef __PIONNER_LIGHT_COMP_H__
#define __PIONNER_LIGHT_COMP_H__

#include <glm/glm.hpp>
#include <memory>

#include "Comp.h"

#include "render/scene/light/LightDef.h"

namespace pio
{
	class RenderEntity;
	class LightComp : public Comp
	{
	public:
		LightComp();
		virtual ~LightComp();

	public:
		glm::vec3 m_pos, m_dir;
		/**
		 * @brief: m_ka, m_kd and m_ks are the color of ambient, diffuse and specular.
		 *         They are RGB format, and range in [0, 1].
		 */
		glm::vec3 m_ka, m_kd, m_ks;
		/**
		 * @brief: m_ia, m_id and m_is are the intensity of ambient, diffuse and specular.
		 *         Their value should be in the range of [0, 1].
		 */
		float m_ia, m_id, m_is;
		/**
		 * @brief: m_shininess is used to control the radius of specular.
		 *         It is used as an exponent to calculate the power of dot(bisector, normal).
		 */
		float m_shininess;

		std::shared_ptr<RenderEntity> m_obj;

		LightType m_type;
	};
}

#endif