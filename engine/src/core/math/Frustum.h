#ifndef __PIONNER_GFX_STRUCT_FRUSTUM_H__
#define __PIONNER_GFX_STRUCT_FRUSTUM_H__

#include "core/Base.h"

namespace pio
{
	enum FrustumDataBit : uint8_t
	{
		FM_BIT_PRJ,
		FM_BIT_ORTHO,
		FM_DATA_BITS
	};

	class Frustum
	{
		friend class Camera;
	public:
		Frustum();
		Frustum(const Frustum &rhs);
		Frustum &operator=(const Frustum &rhs);
		~Frustum() = default;

	private:
		void setFov(float fov);
		void setAspect(float aspect);
		void setNearFar(float near, float far);
		void setOrtho(float l, float r, float b, float t);

		void calcPrj();
		void calcOrtho();

		float near() const { return m_near; }
		float far() const { return m_far; }
		float fov() const { return m_fov; }
		float aspect() const { return m_aspect; };

		float top() const { return m_near * std::tan(glm::radians(m_fov * 0.5f)); }
		float bottom() const { return -top(); }
		float right() const { return top() * m_aspect; }
		float left() const { return -right(); }

		const glm::mat4 &getPerspectMat() const { return m_perspectMat; }
		const glm::mat4 &orthoMat() const { return m_orthoMat; }
		
	private:

		float m_fov{ 60.f }, m_aspect{ 1.f };
		float m_near{ 0.1f }, m_far{ 30.f };

		float m_orthoLeft{ 0.f }, m_orthoRight{ 0.f };
		float m_orthoBottom{ 0.f }, m_orthoTop{ 0.f };

		glm::mat4 m_perspectMat{ 1.f };
		glm::mat4 m_orthoMat{ 1.f };

		std::bitset<FM_DATA_BITS> m_dirty{};
	};
}

#endif