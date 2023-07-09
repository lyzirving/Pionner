#ifndef __PIONNER_SCENEGRRAPH_VIEW_CAMERA_FRUSTUM_H__
#define __PIONNER_SCENEGRRAPH_VIEW_CAMERA_FRUSTUM_H__

#include <bitset>
#include <glm/glm.hpp>

namespace pio
{
	namespace sgf
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
			void setFov(float fov);
			void setAspect(float aspect);
			void setNearFar(float near, float far);
			void setOrtho(float l, float r, float b, float t);

			const glm::mat4 &getPerspectMat();
			const glm::mat4 &getOrthoMat();

			inline float near() { return m_near; }
			inline float far() { return m_far; }
			inline float fov() { return m_fov; }
			inline float aspect() { return m_aspect; };

		private:
			Frustum();
			~Frustum();

			void calcPrj();
			void calcOrtho();

		private:

			float m_fov { 60.f }, m_aspect{ 1.f };
			float m_near{ 0.1f }, m_far{ 30.f };

			float m_orthoLeft  { 0.f }, m_orthoRight{ 0.f };
			float m_orthoBottom{ 0.f }, m_orthoTop{ 0.f };

			glm::mat4 m_perspectMat{ 1.f };
			glm::mat4 m_orthoMat   { 1.f };

			std::bitset<FM_DATA_BITS> m_dirty{};
		};
	}
}

#endif