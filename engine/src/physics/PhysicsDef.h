#ifndef __PIONNER_PHYSICS_PHYSICS_DEF_H__
#define __PIONNER_PHYSICS_PHYSICS_DEF_H__

#include "core/Base.h"

namespace pio
{
	class PhysicsActor;
	class PhysicsScene;

	struct PhysicsSceneDesc
	{
	};

	struct HitResult
	{
		bool Hit{ false };
		uint32_t HitCnt{ 0 };
		PhysicsActor *Actor{ nullptr };// Current implementation supports one hit actor
	};

	enum PhysicsShapeFlag : uint8_t
	{
		Visualize = 0, Query, Simulate, Trigger,
	};

	enum class ColliderType : uint8_t
	{
		None, Box, Num
	};

	enum class PhysicsMatType : uint8_t
	{
		Normal, Num
	};

	#define PIO_NORMAL_DENSITY (10.f)

	#define PIO_COLLIDER_IMPL(x)  public:\
                                  virtual const char *getShapeName() const override { return #x; }\
                                  virtual ColliderType getShapeType() const override { return x; }
}

#endif