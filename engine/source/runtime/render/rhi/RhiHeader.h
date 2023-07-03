#ifndef __RHI_HEADER_H__
#define __RHI_HEADER_H__

#ifndef RHI_USE_OPENGL
#define RHI_USE_OPENGL
#include "render/rhi/opengl/GLHelper.h"
#endif // !RHI_USE_OPENGL

#ifdef RHI_USE_OPENGL
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#endif


#endif