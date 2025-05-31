#include "Transform3D.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Transform3D"

namespace pio
{
	Transform3D::Transform3D()
	{
		m_Attrs.set();
	}

	Transform3D::Transform3D(const glm::vec3& pos, const glm::vec3& euler)
	{
		m_Position = pos;
		m_Rotator = euler;
	}

	Transform3D::Transform3D(const Transform3D& rhs)
	{
		m_Position = rhs.m_Position;
		m_Scale = rhs.m_Scale;
		m_Rotator = rhs.m_Rotator;
	}

	Transform3D::Transform3D(Transform3D&& rhs) noexcept
	{
		m_Position = std::move(rhs.m_Position);
		m_Scale = std::move(rhs.m_Scale);
		m_Rotator = std::move(rhs.m_Rotator);
	}

	Transform3D& Transform3D::operator=(const Transform3D& rhs)
	{
		if (this != &rhs)
		{
			this->Transform3D::Transform3D(rhs);
		}
		return *this;
	}

	Transform3D& Transform3D::operator=(Transform3D&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->Transform3D::Transform3D(std::forward<Transform3D>(rhs));
		}
		return *this;
	}

	Transform3D Transform3D::operator*(const Transform3D& rhs)
	{
		Transform3D t(*this);
		t.m_Position += rhs.m_Position;
		t.m_Scale *= rhs.m_Scale;
		t.m_Rotator += rhs.m_Rotator;
		t.m_Attrs.set();
		return t;
	}

	bool Transform3D::operator==(const Transform3D& rhs) const
	{
		return this != &rhs &&
			this->m_Position == rhs.m_Position&&
			this->m_Scale == rhs.m_Scale &&
			this->m_Rotator == rhs.m_Rotator;
	}

	bool Transform3D::operator!=(const Transform3D& rhs) const
	{
		return !((*this) == rhs);
	}

	bool Transform3D::AddTranslation(const glm::vec3& delta)
	{
		return SetPosition(m_Position + delta);
	}

	bool Transform3D::AddScale(const glm::vec3& delta)
	{
		return SetScale(m_Scale * delta);
	}

	bool Transform3D::AddRotation(const glm::vec3& delta)
	{
		auto euler = m_Rotator + delta;
		return SetRotation(euler.Angle());
	}

	bool Transform3D::SetPosition(const glm::vec3& position)
	{
		if (m_Position != position)
		{
			m_Position = position;
			m_Attrs.set(Attrs_Translation);			
		}
		return m_Attrs.test(Attrs_Translation);
	}

	bool Transform3D::SetScale(const glm::vec3& scale)
	{
		if (m_Scale != scale)
		{
			m_Scale = scale;
			m_Attrs.set(Attrs_Scale);
		}
		return m_Attrs.test(Attrs_Scale);
	}

	bool Transform3D::SetRotation(const glm::vec3& rotation)
	{
		if (m_Rotator != rotation)
		{
			m_Rotator = rotation;
			m_Attrs.set(Attrs_Rotation);
		}
		return m_Attrs.test(Attrs_Rotation);
	}

	void Transform3D::Flush() const
	{
		if (m_Attrs.any())
		{
			m_TranslateMat = glm::translate(glm::mat4(1.f), m_Position);
			m_ScaleMat = glm::scale(glm::mat4(1.f), m_Scale);
			m_TransformMat = m_TranslateMat * m_Rotator.Mat() * m_ScaleMat;
			m_Attrs.reset();
		}
	}

	const glm::mat4& Transform3D::TranslateMat() const
	{
		Flush();
		return m_TranslateMat;
	}

	const glm::mat4& Transform3D::ScaleMat() const
	{
		Flush();
		return m_ScaleMat;
	}

	const glm::mat4& Transform3D::RotMat() const
	{
		Flush();
		return m_Rotator.Mat();
	}

	const glm::mat4& Transform3D::TransformMat() const
	{
		Flush();
		return m_TransformMat;
	}
}