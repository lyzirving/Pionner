#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include "function/framework/components/Component.h"
#include "core/math/Transform.h"

namespace Pionner
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent();
		virtual ~TransformComponent();

		inline bool isDirty() const { return m_dirty; }
		inline void setDirty(bool dirty) { m_dirty = dirty; }
		inline const glm::vec3& getPosition() { return m_transform[m_curIndex].getPosition(); }
		inline const glm::vec3& getScale() { return m_transform[m_curIndex].getScale(); }
		inline const std::pair<float, bool> getRotateByX() { return m_transform[m_curIndex].getRotateByX(); }
		inline const std::pair<float, bool> getRotateByY() { return m_transform[m_curIndex].getRotateByY(); }
		inline const std::pair<float, bool> getRotateByZ() { return m_transform[m_curIndex].getRotateByZ(); }
		inline const glm::mat4& getMatrix() { return m_transform[m_curIndex].matrix(); }
		
		virtual void postLoadResource(const std::weak_ptr<GameObject>& parent) override;
		virtual void tick(float delta) override;

		void setPosition(const glm::vec3 pos);
		void setScale(const glm::vec3 scale);
		void rotateByX(float angle, bool reverse = false);
		void rotateByY(float angle, bool reverse = false);
		void rotateByZ(float angle, bool reverse = false);

	private:
		Transform m_transform[2];
		int       m_curIndex;
		int       m_nextIndex;
		bool      m_dirty;
	};
}

#endif