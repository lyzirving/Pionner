#include "UniformBlock.h"

namespace pio
{
	Ref<UniformStorageLayout> UniformBlock::CreateData(UniformType type, const std::string& name, uint32_t arrayNum)
	{
		switch (type)
		{
			case UniformType::Bool:
			case UniformType::Float:
			case UniformType::Int:
			case UniformType::UInt:
			case UniformType::Vec2:
			case UniformType::Vec3:
			case UniformType::Vec4:
			case UniformType::IVec2:
			case UniformType::IVec3:
			case UniformType::IVec4:
			case UniformType::UVec2:
			case UniformType::UVec3:
			case UniformType::UVec4:
				return CreateRef<UniformStorageLayout>(type, name);
			case UniformType::Mat2:
				return CreateRef<UniformStorageLayout, UniformMatLayout>(type, 2, name);
			case UniformType::Mat3:
				return CreateRef<UniformStorageLayout, UniformMatLayout>(type, 3, name);
			case UniformType::Mat4:
				return CreateRef<UniformStorageLayout, UniformMatLayout>(type, 4, name);
			case UniformType::FloatArray:
			case UniformType::IntArray:
			case UniformType::UIntArray:
			case UniformType::Vec2Array:
			case UniformType::Vec3Array:
			case UniformType::Vec4Array:
				return CreateRef<UniformStorageLayout, UniformArrayLayout>(type, arrayNum, name);
			case UniformType::Mat2Array:
			case UniformType::Mat3Array:
			case UniformType::Mat4Array:
				return CreateRef<UniformStorageLayout, UniformMatArrayLayout>(type, arrayNum, name);
			case UniformType::Struct:
				return CreateRef<UniformStorageLayout, UniformStructLayout>(name);
			case UniformType::StructArray:
				return CreateRef<UniformStorageLayout, UniformStructArrayLayout>(name);
			default:
				LOGE("err! invalid data type[%u]", type);
				std::abort();
				return Ref<UniformStorageLayout>();
		}
	}

	UniformBlock::UniformBlock()
	{
	}

	UniformBlock::~UniformBlock() = default;

	void UniformBlock::Calculate()
	{
		uint32_t offset{ 0 };
		uint32_t byteUsed{ 0 };
		for (size_t i = 0; i < m_BlockItems.Size(); i++)
		{
			auto& item = m_BlockItems[i];
			item->CalculateOffset(byteUsed, offset, (i == 0));
			offset = item->GetAlignOffset();
			byteUsed = item->GetByteUsed();
			if (i == 0)
				m_AlignOffset = offset;

			// [Note]: in some case, i can be both the first one and last one
			if (i == m_BlockItems.Size() - 1)
				m_ByteUsed = offset + byteUsed - m_AlignOffset;
		}
		m_Buffer = CreateRef<Buffer>();
		m_Buffer->allocate(0, m_ByteUsed);
	}

	void UniformBlock::WriteAt(const void* data, uint64_t size, uint64_t offset)
	{
		m_Buffer->WriteAt(data, size, offset);
		Invalidate();
	}

	std::string UniformBlock::ToString()
	{
		std::stringstream ss;
		ss << "\n---------- block align offset[" << m_AlignOffset << "], byte used[" << m_ByteUsed << "] ----------";
		for (size_t i = 0; i < m_BlockItems.Size(); i++)
		{
			ss << "\n";
			ss << m_BlockItems[i]->ToString();
		}
		return ss.str();
	}	
}