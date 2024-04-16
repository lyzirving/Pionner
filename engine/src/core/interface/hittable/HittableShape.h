#ifndef __PIONNER_CORE_INTERFACE_HITTABLE_HITTABLE_SHAPE_H__
#define __PIONNER_CORE_INTERFACE_HITTABLE_HITTABLE_SHAPE_H__

#include "Hittable.h"

#include "core/interface/Movable.h"

#include "gfx/struct/Geometry2D.h"
#include "gfx/renderer/RenderData.h"

namespace pio
{
	enum class HitShapeType : uint8_t
	{
		None, Box
	};

	#define OVERRIDE_HITTABLE_SHAPE(type)  public:\
									       static  HitShapeType GetStaticShapeType() { return type; }\
									       virtual HitShapeType getShapeType() const override { return GetStaticShapeType(); }

	class HittableShape : public Hittable, public Movable
	{
	public:
		HittableShape() : Hittable(), Movable() {}
		virtual ~HittableShape() = default;

		virtual void onCreateShape() = 0;
		virtual void onDraw(const DrawParam &param) = 0;

		virtual HitShapeType getShapeType() const { return HitShapeType::None; }

		const Ref<LineMesh> &getOutline() { return m_outline; }
		const Ref<LineMesh> &getOutline() const { return m_outline; }

	protected:
		HitShapeType  m_type{ HitShapeType::None };
		Ref<LineMesh> m_outline;
	};
}

#endif