#pragma once

#include "base/TypeDef.h"
#include "MathDef.h"

namespace pio
{
	/*
	 * Point layout of a AABB
	 *       p7 ----------- p6
	 *      /|            / |
	 *     / |           /  |
	 *   p4 -|--------- p5  |
	 *    |  p3 --------|-- p2
	 *    | /           |  /
	 *    |/            | /
	 *	  /             |/
	 *    p0 ---------- p1
	 */
	class AABB
	{
	public:
		AABB() {}
		AABB(const glm::vec3& min, const glm::vec3& max) : m_Min(min), m_Max(max) { UpdateCorners(); }

		bool operator==(const AABB& rhs) const 
		{
			return m_Min == rhs.m_Min && m_Max == rhs.m_Max;
		}

		bool operator!=(const AABB& rhs) const
		{
			return !((*this) == rhs);
		}

		const glm::vec3& GetMin() const { return m_Min; }
		const glm::vec3& GetMax() const { return m_Max; }

		void Update(const glm::vec3& p)
		{
			m_Min = glm::min(m_Min, p);
			m_Max = glm::max(m_Max, p);
		}		

		bool Contain(const glm::vec3& p) const
		{
			return p > m_Min && p < m_Max;
		}

		bool Contain(const AABB& rhs) const
		{
			return m_Min < rhs.m_Min && rhs.m_Max < m_Max;
		}

		bool IsIntersect(const AABB& rhs) const
		{
			if(m_Min.x > rhs.m_Max.x || m_Max.x < rhs.m_Min.x || 
			   m_Min.y > rhs.m_Max.y || m_Max.y < rhs.m_Min.y ||
			   m_Min.z > rhs.m_Max.z || m_Max.z < rhs.m_Min.z)
			{
				return false;
			}
			return true;		
		}		

		glm::vec3 Center() const { return (m_Min + m_Max) * 0.5f; } 
		//[NOTE]: It's weird that some model's max is smaller than min after model transformation.
		//        So i use std::abs() to make sure the result is positive
		float LengthX() const { return std::abs(m_Max.x - m_Min.x); }
		float LengthY() const { return std::abs(m_Max.y - m_Min.y); }
		float LengthZ() const { return std::abs(m_Max.z - m_Min.z); }
		float Diagonal() const { return glm::distance(m_Max, m_Min); }

		bool IsValid() const { return m_Min <= m_Max; }
		void Reset() { m_Min = glm::vec3(FLT_MAX); m_Max = glm::vec3(-FLT_MAX); }

		std::string ToString() const 
		{
			std::string result("Min[");
			result.append(std::to_string(m_Min.x)).append(", ").
				append(std::to_string(m_Min.y)).append(", ").
				append(std::to_string(m_Min.z)).append("], Max[").
				append(std::to_string(m_Max.x)).append(", ").
				append(std::to_string(m_Max.y)).append(", ").
				append(std::to_string(m_Max.z)).append("]");
			return result;
		}

		void Union(const AABB& rhs)
		{
			if(!rhs.IsValid())
				return;

			if(IsValid())
			{
				m_Min = glm::min(m_Min, rhs.m_Min);
				m_Max = glm::max(m_Max, rhs.m_Max);
			}
			else
			{
				m_Min = rhs.m_Min;
				m_Max = rhs.m_Max;
			}
		}

		void Intersect(const AABB& rhs)
		{
		}

	private:
		void UpdateCorners()
		{			
			m_Points[3] = m_Min;
			m_Points[5] = m_Max;

			m_Points[0] = m_Points[3] + World::Forward * LengthZ();
			m_Points[1] = m_Points[0] + World::Right * LengthX();
			m_Points[2] = m_Points[3] + World::Right * LengthX();

			m_Points[4] = m_Points[5] - World::Right * LengthX();
			m_Points[6] = m_Points[5] - World::Forward * LengthZ();
			m_Points[7] = m_Points[4] - World::Forward * LengthZ();
		}

		void UpdateMinMax()
		{
			for(size_t i = 0; i < 8; i++)
			{
				Update(m_Points[i]);
			}
		}

	private:
		friend AABB operator*(const glm::mat3& matrix, const AABB& aabb);
		friend AABB operator*(const glm::mat4& matrix, const AABB& aabb);

	private:
		//m_Min refers to p3, m_Max refers to p5 in current coordinate system
		glm::vec3 m_Min{ FLT_MAX }, m_Max{ -FLT_MAX };
		glm::vec3 m_Points[8]{ glm::vec3(0.f) };
	};

	inline AABB operator*(const glm::mat3& matrix, const AABB& aabb)
	{
		AABB result(aabb.m_Min, aabb.m_Max);
		for(size_t i = 0; i < 8; i++)
		{
			result.m_Points[i] = matrix * result.m_Points[i];	
		}
		result.Reset();
		result.UpdateMinMax();
		return result;
	}

	inline AABB operator*(const glm::mat4& matrix, const AABB& aabb)
	{
		AABB result(aabb.m_Min, aabb.m_Max);
		for(size_t i = 0; i < 8; i++)
		{
			result.m_Points[i] = matrix * glm::vec4(result.m_Points[i], 1.f);
		}
		result.Reset();
		result.UpdateMinMax();
		return result;
	}
}