#ifndef __RHI_STRUCT_H__
#define __RHI_STRUCT_H__

namespace Pionner
{
	struct RhiViewport
	{
		float x, y;
		float width, height;
		float minDepth, maxDepth;
	};
}

#endif