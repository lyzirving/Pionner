#ifndef __RENDER_SCENE_DEF_H__
#define __RENDER_SCENE_DEF_H__

#include <cstdint>

namespace Pionner
{
	enum SceneLayerType : uint8_t
	{
		LAYER_CLEAR,
		LAYER_MODEL,
		LAYER_COUNT
	};
}

#endif	