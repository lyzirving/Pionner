#pragma once

#include "UBufferProxy.h"

namespace pio
{
	class UMotionBuffer : public UBufferProxy
	{
		RTTR_ENABLE(UBufferProxy)
	public:
		UMotionBuffer(const Ref<RenderContext>& context, const std::string& name);
		~UMotionBuffer() = default;		

		void SetTransform(const glm::mat4& matrix);
		void SetLocalTransform(const glm::mat4& matrix);
		void SetSkeletalOn(bool val);
	};
}