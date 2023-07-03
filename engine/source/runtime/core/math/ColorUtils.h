#ifndef __MATH_COLOR_UTILS_H__
#define __MATH_COLOR_UTILS_H__

namespace pio
{

#define RED_CHANNEL(color)    (((color >> 24u) & 0xffu) * 1.f / 255.f)
#define GREEN_CHANNEL(color)  (((color >> 16u) & 0xffu) * 1.f / 255.f)
#define BLUE_CHANNEL(color)   (((color >> 8u) & 0xffu) * 1.f / 255.f)
#define APLHA_CHANNEL(color)  ((color & 0xffu) * 1.f / 255.f)

}

#endif