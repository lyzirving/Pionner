#ifndef __PIONNER_BASE_TRANSFORM_H__
#define __PIONNER_BASE_TRANSFORM_H__

#include <bitset>
#include "EulerAngle.h"

namespace pio
{
	class Transform3D
	{
	public:
		Transform3D();
		Transform3D(const glm::vec3& pos, const glm::vec3& euler);
		~Transform3D() = default;
		Transform3D(const Transform3D& rhs);
		Transform3D(Transform3D&& rhs) noexcept;

		Transform3D& operator=(const Transform3D& rhs);
		Transform3D& operator=(Transform3D&& rhs) noexcept;

		Transform3D operator*(const Transform3D& rhs);

		bool addTranslation(const glm::vec3& delta);
		bool addScale(const glm::vec3& delta);
		bool addEuler(const glm::vec3& delta);

		bool setPosition(const glm::vec3& position);
		bool setScale(const glm::vec3& scale);
		bool setEuler(const glm::vec3& euler);

		const glm::vec3& position() const{ return m_position; }
		const glm::vec3& scale() const { return m_scale; }
		const glm::vec3& euler() const { return m_euler.angle(); }

		void flush() const;
		const glm::mat4& translateMat() const;
		const glm::mat4& scaleMat() const;
		const glm::mat4& rotMat() const;
		const glm::mat4& transformMat() const;		

	private:
		enum Attrs : uint8_t
		{
			Attrs_Translation,
			Attrs_Scale,
			Attrs_Rotation,
			Attrs_Num
		};

	private:		
		glm::vec3 m_position{ 0.f }, m_scale{ 1.f };
		EulerAngle m_euler;

		mutable glm::mat4 m_translateMat{ 1.f }, m_scaleMat{ 1.f };
		mutable glm::mat4 m_transformMat{ 1.f };
		mutable std::bitset<Attrs_Num> m_attrs;
	};
}

#endif