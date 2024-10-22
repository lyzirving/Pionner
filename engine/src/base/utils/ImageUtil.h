#ifndef __PIONNER_BASE_UTILS_IMAGE_UTIL_H__
#define __PIONNER_BASE_UTILS_IMAGE_UTIL_H__

#include "LogSystem.h"

namespace pio
{
	class ImageUtil
	{
	public:
		static bool GetPicInfo(const char* filePath, int32_t& w, int32_t& h, int32_t& comp);		
		static bool LoadImage(const std::string& path, void** data, int32_t& w, int32_t &h, int32_t& comp, int32_t reqComp = 0);
		static void FreeImage(void** data);

		template<typename T>
		static bool FillSingleChannel(T* src, int32_t channel, int32_t req, int32_t w, int32_t h, const T& val)
		{
			if (!src)
			{
				#ifdef LOCAL_TAG
				#undef LOCAL_TAG
				#endif
				#define LOCAL_TAG "ImageUtil"
				LOGE("invalid src");
				return false;
			}

			if (req < 0 || req >= channel || channel > 4 || channel <= 0)
			{
				#ifdef LOCAL_TAG
				#undef LOCAL_TAG
				#endif
				#define LOCAL_TAG "ImageUtil"
				LOGE("invalid channel[%d] or req[%d]", channel, req);
				return false;
			}

			for (uint32_t i = 0; i < h; i++)
			{
				for (uint32_t j = 0; j < w; j++)
				{
					src[(i * w + j) * channel + req] = val;
				}
			}
		}

		template<typename T>
		static bool FillChannelData(T* src, int32_t srcChannel, int32_t srcReq,
									T* dst, int32_t dstChannel, int32_t dstReq,
									int32_t w, int32_t h, bool flip = false, T limit = 0)
		{
			if (!src || !dst)
			{
				#ifdef LOCAL_TAG
				#undef LOCAL_TAG
				#endif
				#define LOCAL_TAG "ImageUtil"
				LOGE("invalid src or dst");
				return false;
			}
			if (srcReq < 0 || srcReq > 3 || dstReq < 0 || dstReq > 3)
			{
				#ifdef LOCAL_TAG
				#undef LOCAL_TAG
				#endif
				#define LOCAL_TAG "ImageUtil"
				LOGE("invalid req, src[%d], dst[%d]", srcReq, dstReq);
				return false;
			}
			for (uint32_t i = 0; i < h; i++)
			{
				for (uint32_t j = 0; j < w; j++)
				{
					T val = src[(i * w + j) * srcChannel + srcReq];
					dst[(i * w + j) * dstChannel + dstReq] = flip ? (limit - val) : val;
				}
			}
			return true;
		}
	};
}

#endif