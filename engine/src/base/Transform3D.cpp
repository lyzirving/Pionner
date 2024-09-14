#include "Transform3D.h"
#include "Transform3D.h"

namespace pio
{
	Transform3D::Transform3D()
	{
		m_attrs.set();
	}

	Transform3D::Transform3D(const glm::vec3& pos, const glm::vec3& euler)
	{
		m_position = pos;
		m_euler = euler;
	}

	Transform3D::Transform3D(const Transform3D& rhs)
	{
		m_position = rhs.m_position;
		m_scale = rhs.m_scale;
		m_euler = rhs.m_euler;
	}

	Transform3D::Transform3D(Transform3D&& rhs) noexcept
	{
		m_position = std::move(rhs.m_position);
		m_scale = std::move(rhs.m_scale);
		m_euler = std::move(rhs.m_euler);
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
		t.m_position += rhs.m_position;
		t.m_scale *= rhs.m_scale;
		t.m_euler += rhs.m_euler;
		t.m_attrs.set();
		return t;
	}

	bool Transform3D::addTranslation(const glm::vec3& delta)
	{
		return setPosition(m_position + delta);
	}

	bool Transform3D::addScale(const glm::vec3& delta)
	{
		return setScale(m_scale + delta);
	}

	bool Transform3D::addEuler(const glm::vec3& delta)
	{
		auto euler = m_euler + delta;
		return setEuler(euler.angle());
	}

	bool Transform3D::setPosition(const glm::vec3& position)
	{
		if (m_position != position)
		{
			m_position = position;
			m_attrs.set(Attrs_Translation);			
		}
		return m_attrs.test(Attrs_Translation);
	}

	bool Transform3D::setScale(const glm::vec3& scale)
	{
		if (m_scale != scale)
		{
			m_scale = scale;
			m_attrs.set(Attrs_Scale);
		}
		return m_attrs.test(Attrs_Scale);
	}

	bool Transform3D::setEuler(const glm::vec3& euler)
	{
		if (m_euler != euler)
		{
			m_euler = euler;
			m_attrs.set(Attrs_Rotation);
		}
		return m_attrs.test(Attrs_Rotation);
	}

	void Transform3D::flush() const
	{
		if (m_attrs.any())
		{
			m_translateMat = glm::translate(glm::mat4(1.f), m_position);
			m_scaleMat = glm::scale(glm::mat4(1.f), m_scale);
			m_transformMat = m_translateMat * m_euler.mat() * m_scaleMat;
			m_attrs.reset();
		}
	}

	const glm::mat4& Transform3D::translateMat() const
	{
		flush();
		return m_translateMat;
	}

	const glm::mat4& Transform3D::scaleMat() const
	{
		flush();
		return m_scaleMat;
	}

	const glm::mat4& Transform3D::rotMat() const
	{
		flush();
		return m_euler.mat();
	}

	const glm::mat4& Transform3D::transformMat() const
	{
		flush();
		return m_transformMat;
	}
}