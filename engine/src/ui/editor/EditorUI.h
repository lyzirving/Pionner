#ifndef __PIONNER_UI_EDITOR_EDITOR_UI_H__
#define __PIONNER_UI_EDITOR_EDITOR_UI_H__

#include "asset/Asset.h"

namespace pio
{
	enum EditorAxis : uint8_t
	{
		EditorAxis_X, EditorAxis_Y, EditorAxis_Z, EditorAxis_Cnt
	};

	class EditorUI
	{
	public:
		EditorUI() { onCreateMesh(); }
		virtual ~EditorUI() = default;

		virtual void onCreateMesh() = 0;
		virtual void onDraw() = 0;

		void setTransform(const glm::mat4 &mat) { m_transform = mat; }

		const glm::mat4 &getTransform() { return m_transform; }
		const glm::mat4 &getTransform() const { return m_transform; }

	protected:
		glm::mat4 m_transform{ 1.f };
	};
}

#endif