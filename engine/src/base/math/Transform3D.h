#pragma once

#include <bitset>
#include "Rotator.h"

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

		bool operator==(const Transform3D& rhs) const;
		bool operator!=(const Transform3D& rhs) const;

		bool AddTranslation(const glm::vec3& delta);
		bool AddScale(const glm::vec3& delta);
		bool AddRotation(const glm::vec3& delta);

		bool SetPosition(const glm::vec3& position);
		bool SetScale(const glm::vec3& scale);
		bool SetRotation(const glm::vec3& rotation);

		const glm::vec3& Position() const{ return m_Position; }
		const glm::vec3& Scale() const { return m_Scale; }
		const glm::vec3& Rotation() const { return m_Rotator.Angle(); }

		void Flush() const;
		const glm::mat4& TranslateMat() const;
		const glm::mat4& ScaleMat() const;
		const glm::mat4& RotMat() const;
		const glm::mat4& TransformMat() const;		

	private:
		enum Attrs : uint8_t
		{
			Attrs_Translation,
			Attrs_Scale,
			Attrs_Rotation,
			Attrs_Num
		};

	private:		
		glm::vec3 m_Position{ 0.f };
		glm::vec3 m_Scale{ 1.f };
		Rotator m_Rotator;

		mutable glm::mat4 m_TranslateMat{ 1.f };
		mutable glm::mat4 m_ScaleMat{ 1.f };
		mutable glm::mat4 m_TransformMat{ 1.f };
		mutable std::bitset<Attrs_Num> m_Attrs;
	};
}