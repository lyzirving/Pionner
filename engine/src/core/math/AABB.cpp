#include "AABB.h"

#include "asset/AssetsManager.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/struct/Geometry2D.h"

namespace pio
{
	AABB::AABB()
	{
		onCreateMesh();
	}

	AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) : Min(min), Max(max)
	{
		onCreateMesh();
	}

	void AABB::onCreateMesh()
	{
		//TODO: create mesh for outline
	}

	void AABB::onDraw(const DrawParam &param)
	{
		updatePoints();
		//TODO: draw outline of AABB
	}
}