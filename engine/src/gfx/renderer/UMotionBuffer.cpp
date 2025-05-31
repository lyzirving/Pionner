#include "UMotionBuffer.h"

namespace pio
{
	UMotionBuffer::UMotionBuffer(const Ref<RenderContext>& context, const std::string& name)
		: UBufferProxy(context, name)
	{
		m_UBlock = CreateRef<UniformBlock>();
		m_UBlock->PushBack("Transform", UniformBlock::CreateData(UniformType::Mat4, "Transform"));
		m_UBlock->PushBack("LocalTransform", UniformBlock::CreateData(UniformType::Mat4, "LocalTransform"));
		m_UBlock->PushBack("bSkeletalOn", UniformBlock::CreateData(UniformType::Bool, "bSkeletalOn"));
		m_UBlock->Calculate();

		m_UBuffer = UniformBuffer::Create(m_Context.lock(), m_UBlock->GetByteUsed(),
										  UBBinging_Motion, BufferUsage::Dynamic);
	}

	void UMotionBuffer::SetTransform(const glm::mat4& matrix)
	{
		auto layout = (*m_UBlock)["Transform"];
		m_UBlock->WriteAt(glm::value_ptr(matrix), sizeof(glm::mat4), layout->GetAlignOffset());
	}

	void UMotionBuffer::SetLocalTransform(const glm::mat4& matrix)
	{
		auto layout = (*m_UBlock)["LocalTransform"];
		m_UBlock->WriteAt(glm::value_ptr(matrix), sizeof(glm::mat4), layout->GetAlignOffset());
	}

	void UMotionBuffer::SetSkeletalOn(bool val)
	{
		auto layout = (*m_UBlock)["bSkeletalOn"];
		m_UBlock->WriteAt(&val, sizeof(bool), layout->GetAlignOffset());
	}
}