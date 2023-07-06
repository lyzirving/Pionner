#ifndef __RHI_HEADER_H__
#define __RHI_HEADER_H__

#ifdef RHI_USE_OPENGL
#include "render/rhi/opengl/GLHelper.h"
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#endif

#endif