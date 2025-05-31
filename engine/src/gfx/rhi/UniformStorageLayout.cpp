#include "UniformStorageLayout.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformStorageLayout"

namespace pio
{
	const char* GetBindingName(UBBinding binding)
	{
		switch (binding)
		{
			case UBBinding_Camera:
				return "CameraBlock";
			case UBBinding_PBRMaterial:
				return "PBRBlock";
			case UBBinding_TexturedMaterial:
				return "TexturedBlock";
			case UBBinding_DirectionalLit:
				return "DirectionalLitBlock";
			case UBBinding_DirectionalLitShadow:
				return "DirectionalLitShadowBlock";
			case UBBinding_PointLit:
				return "PointLitBlock";
			case UBBinding_PointLitShadow:
				return "PointLitShadowCasterBlock";
			case UBBinging_Motion:
				return "MotionBlock";
			case UBBinging_Skeleton:
				return "SkeletonBlock";
			default:
				LOGE("err! unknown binding[%u]!", binding);
				std::abort();
				return "Unknown_Binding";
		}
	}

	// ------------------------ UniformStorageLayout -------------------------
	// --------------------------------------------------------------
	bool UniformStorageLayout::IsPrimitive(UniformType type)
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
				return true;
			default:
				return false;;
		}
	}

	uint32_t UniformStorageLayout::CalcAlignOffset(uint32_t occupied, uint32_t baseAlign)
	{
		if ((occupied % baseAlign) == 0)
			return occupied;

		uint32_t i = 1;
		uint32_t offset = i * baseAlign;
		while (offset < occupied)
		{
			i++;
			offset = i * baseAlign;
		}
		return offset;
	}

	UniformStorageLayout::UniformStorageLayout()
	{
	}

	UniformStorageLayout::UniformStorageLayout(UniformType type) : m_Type(type)
	{
	}

	UniformStorageLayout::UniformStorageLayout(UniformType type, const std::string &name) : m_Type(type), m_Name(name)
	{
	}

	void UniformStorageLayout::FillMetaData()
	{
		if (m_BaseAlign != 0)
			return;

		m_BaseAlign = GetUniformBaseAlign(m_Type);
		m_ByteUsed = GetUniformColumnsByteSize(m_Type);
	}

	void UniformStorageLayout::CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		FillMetaData();
		m_AlignOffset = blockStart ? 0 : CalcAlignOffset(byteUsed + offset, m_BaseAlign);
	}

	std::string UniformStorageLayout::ToString() const
	{
		std::stringstream ss{};
		ss << "name[" << m_Name << "], base align[" << m_BaseAlign << "], align offset[" << m_AlignOffset << "], byte used[" << m_ByteUsed << "]";
		return ss.str();
	}

	// ------------------------ UniformMatLayout --------------------------
	// --------------------------------------------------------------
	UniformType UniformMatLayout::GetColumnDataType(UniformType type)
	{
		switch (type)
		{
			case UniformType::Mat2:
				return UniformType::Vec2;
			case UniformType::Mat3:
				return UniformType::Vec3;
			case UniformType::Mat4:
				return UniformType::Vec4;
			default:
				LOGE("UniformMatLayout: invalid Mat type[%u]", type);
				std::abort();
				return UniformType::Vec2;
		}
	}

	uint32_t UniformMatLayout::GetMatColumnNum(UniformType type)
	{
		switch (type)
		{
			case UniformType::Mat2:
				return 2;
			case UniformType::Mat3:
				return 3;
			case UniformType::Mat4:
				return 4;
			default:
				LOGE("UniformMatLayout: invalid Mat type[%u]", type);
				std::abort();
				return 0;
		}
	}

	UniformMatLayout::UniformMatLayout() : UniformStorageLayout()
	{
	}

	UniformMatLayout::UniformMatLayout(UniformType type, uint32_t columnNum, const std::string &name)
		: UniformStorageLayout(type, name), m_ColumnNum(columnNum)
	{
		if (m_ColumnNum <= 1)
		{
			//LOGE("input column num[%u] is invald!", m_ColumnNum);
			return;
		}
		FillMetaData();
	}

	UniformMatLayout::~UniformMatLayout() = default;

	void UniformMatLayout::FillMetaData()
	{
		if (m_BaseAlign != 0)
			return;

		m_Columns.resize(m_ColumnNum);
		m_BaseAlign = GetUniformBaseAlign(m_Type);
		for (uint32_t i = 0; i < m_ColumnNum; i++)
		{
			m_Columns[i].m_Type = UniformMatLayout::GetColumnDataType(m_Type);
			m_Columns[i].m_BaseAlign = m_BaseAlign;
			m_Columns[i].m_ByteUsed = GetUniformColumnsByteSize(m_Columns[i].m_Type);
		}
	}

	void UniformMatLayout::CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_ColumnNum <= 1)
		{
			LOGE("invalid column num[%u]!", m_ColumnNum);
			return;
		}

		FillMetaData();

		for (uint32_t i = 0; i < m_Columns.size(); i++)
		{
			auto &col = m_Columns[i];
			col.CalculateOffset(byteUsed, offset, blockStart && (i == 0));
			byteUsed = col.GetByteUsed();
			offset = col.GetAlignOffset();

			if (i == 0)
				m_AlignOffset = col.GetAlignOffset();
		}

		auto &last = m_Columns.back();
		uint32_t lastOccupy = last.GetOccupied();
		// NOTE: matrix has end padding.
		m_EndPadding = CalcAlignOffset(lastOccupy, m_BaseAlign);
		m_ByteUsed = m_EndPadding - m_AlignOffset;
	}

	std::string UniformMatLayout::ToString() const
	{
		std::stringstream ss{};
		ss << "name[" << m_Name << "], base align[" << m_BaseAlign << "]\n";
		for (uint32_t i = 0; i < m_Columns.size(); i++)
		{
			ss << "column[" << i << "], align offset[" << m_Columns[i].GetAlignOffset() << "], byte used[" << m_Columns[i].GetByteUsed() << "], ";
		}
		ss << "\nending padding[" << m_EndPadding << "]";
		return ss.str();
	}

	// ------------------------ UniformArrayLayout --------------------------
	// ----------------------------------------------------------------

	UniformType UniformArrayLayout::GetArrayDataType(UniformType type)
	{
		switch (type)
		{
			case UniformType::Vec2Array:
			case UniformType::Vec3Array:
			case UniformType::Vec4Array:
			case UniformType::FloatArray:
				return UniformType::Float;
			case UniformType::IntArray:
				return UniformType::Int;
			case UniformType::UIntArray:
				return UniformType::UInt;
			default:
				LOGE("UniformMatLayout: invalid array type[%u]", type);
				std::abort();
				return UniformType::Float;
		}
	}

	UniformArrayLayout::UniformArrayLayout(UniformType type, uint32_t arrayNum, const std::string &name)
		: UniformStorageLayout(type, name), m_ArrayNum(arrayNum)
	{
		FillMetaData();
	}

	UniformArrayLayout::~UniformArrayLayout() = default;

	void UniformArrayLayout::FillMetaData()
	{
		if (m_BaseAlign != 0)
			return;

		m_Array.resize(m_ArrayNum);
		m_BaseAlign = GetUniformBaseAlign(m_Type);
		for (uint32_t i = 0; i < m_ArrayNum; i++)
		{
			m_Array[i].m_Type = UniformArrayLayout::GetArrayDataType(m_Type);
			m_Array[i].m_BaseAlign = m_BaseAlign;
			m_Array[i].m_ByteUsed = GetUniformColumnsByteSize(m_Array[i].m_Type);
		}
	}

	void UniformArrayLayout::CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_ArrayNum < 1)
		{
			LOGE("invalid array num[%u]", m_ArrayNum);
			return;
		}

		FillMetaData();

		for (uint32_t i = 0; i < m_Array.size(); i++)
		{
			auto &item = m_Array[i];
			item.CalculateOffset(byteUsed, offset, blockStart && (i == 0));
			byteUsed = item.GetByteUsed();
			offset = item.GetAlignOffset();

			if (i == 0)
				m_AlignOffset = item.GetAlignOffset();
		}

		auto &last = m_Array.back();
		uint32_t lastOccupy = last.GetOccupied();
		// NOTE: array has end padding.
		m_EndPadding = CalcAlignOffset(lastOccupy, m_BaseAlign);
		m_ByteUsed = m_EndPadding - m_AlignOffset;
	}

	std::string UniformArrayLayout::ToString() const
	{
		std::stringstream ss{};
		ss << "name[" << m_Name << "], array base align[" << m_BaseAlign << "]\n";
		for (uint32_t i = 0; i < m_Array.size(); i++)
		{
			ss << "item[" << i << "], align offset[" << m_Array[i].GetAlignOffset()
				<< "], byte used[" << m_Array[i].GetByteUsed() << "], ";
		}
		ss << "\nend padding[" << m_EndPadding << "]";
		return ss.str();
	}
	// ------------------------ UniformMatArrayLayout --------------------------
	// -------------------------------------------------------------------

	UniformType UniformMatArrayLayout::GetArrayDataType(UniformType type)
	{
		switch (type)
		{
			case UniformType::Mat2Array:
				return UniformType::Mat2;
			case UniformType::Mat3Array:
				return UniformType::Mat3;
			case UniformType::Mat4Array:
				return UniformType::Mat4;
			default:
				LOGE("invalid mat array type[%]", type);
				std::abort();
				return UniformType::Mat2;
		}
	}

	UniformMatArrayLayout::UniformMatArrayLayout(UniformType type, uint32_t arrayNum, const std::string &name)
		: UniformStorageLayout(type, name), m_ArrayNum(arrayNum)
	{
		FillMetaData();
	}

	UniformMatArrayLayout::~UniformMatArrayLayout() = default;

	void UniformMatArrayLayout::FillMetaData()
	{
		if (m_BaseAlign != 0)
			return;

		m_MatArray.resize(m_ArrayNum);
		m_BaseAlign = GetUniformBaseAlign(m_Type);
		for (uint32_t i = 0; i < m_ArrayNum; i++)
		{
			m_MatArray[i].m_Type = UniformMatArrayLayout::GetArrayDataType(m_Type);
			m_MatArray[i].m_ColumnNum = UniformMatLayout::GetMatColumnNum(m_MatArray[i].m_Type);
		}
	}

	void UniformMatArrayLayout::CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_MatArray.empty())
			return;

		FillMetaData();

		for (uint32_t i = 0; i < m_MatArray.size(); i++)
		{
			auto &mat = m_MatArray[i];
			mat.CalculateOffset(byteUsed, offset, blockStart && (i == 0));
			byteUsed = mat.GetByteUsed();
			offset = mat.GetAlignOffset();

			if (i == 0)
				m_AlignOffset = mat.GetAlignOffset();
		}

		auto &last = m_MatArray.back();
		uint32_t lastOccupy = last.GetOccupied();
		// NOTE: mat array has end padding.
		m_EndPadding = CalcAlignOffset(lastOccupy, m_BaseAlign);
		m_ByteUsed = m_EndPadding - m_AlignOffset;
	}

	std::string UniformMatArrayLayout::ToString() const
	{
		std::stringstream ss{};
		ss << "name[" << m_Name <<"], base align[" << m_BaseAlign << "]\n";
		for (uint32_t i = 0; i < m_MatArray.size(); i++)
		{
			ss << "item[" << i << "], align offset[" << m_MatArray[i].GetAlignOffset() 
			   << "], byte used[" << m_MatArray[i].GetByteUsed() << "](include padding), ";
		}
		ss << "\nend padding[" << m_EndPadding << "]";
		return ss.str();
	}
	// ------------------------- UniformStructLayout ---------------------------
	// -------------------------------------------------------------------
	UniformStructLayout::UniformStructLayout() : UniformStorageLayout(UniformType::Struct)
	{
	}

	UniformStructLayout::UniformStructLayout(const std::string &name) : UniformStorageLayout(UniformType::Struct, name)
	{
	}

	UniformStructLayout::~UniformStructLayout()
	{
		auto it = m_Data.begin();
		while (it != m_Data.end())
		{
			it->second.reset();
			it = m_Data.erase(it);
		}
	}

	void UniformStructLayout::FillMetaData()
	{
		if (m_BaseAlign != 0 || m_Data.empty())
			return;

		m_BaseAlign = GetUniformBaseAlign(m_Type);
		for (auto &d : m_Data)
		{
			d.second->FillMetaData();
		}
	}

	void UniformStructLayout::CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_Data.empty())
			return;

		FillMetaData();

		if (blockStart)
			m_StartPadding = 0;
		else
			m_StartPadding = UniformStorageLayout::CalcAlignOffset(byteUsed + offset, m_BaseAlign);

		uint32_t tmpOffset = m_AlignOffset = m_StartPadding;
		uint32_t tmpUsed = 0;
		Ref<UniformStorageLayout> last{};
		auto it = m_Data.begin();
		while (it != m_Data.end())
		{
			auto &d = it->second;
			// block start is alreay used
			d->CalculateOffset(tmpUsed, tmpOffset, false);
			tmpUsed = d->GetByteUsed();
			tmpOffset = d->GetAlignOffset();
			last = d;
			it++;
		}

		uint32_t lastOccupy = last->GetOccupied();
		m_EndPadding = CalcAlignOffset(lastOccupy, m_BaseAlign);
		m_ByteUsed = m_EndPadding - m_AlignOffset;
	}

	std::string UniformStructLayout::ToString() const
	{
		std::stringstream ss{};
		ss << "name["<< m_Name << "], base align[" << m_BaseAlign << "], start padding[" << m_StartPadding << "]\n";
		auto it = m_Data.begin();
		while (it != m_Data.end())
		{
			ss << "[" << it->first << "], base align[" << it->second->GetBaseAlign()
				<< "], align offset[" << it->second->GetAlignOffset()
				<< "], byte used[" << it->second->GetByteUsed() << "], ";
			it++;
		}
		ss << "\nend padding[" << m_EndPadding << "]";
		return ss.str();
	}

	// ------------------------- UniformStructArrayLayout ---------------------------
	// ------------------------------------------------------------------------

	UniformStructArrayLayout::UniformStructArrayLayout(const std::string &name)
		: UniformStorageLayout(UniformType::StructArray, name)
	{
	}

	UniformStructArrayLayout::~UniformStructArrayLayout() = default;

	void UniformStructArrayLayout::FillMetaData()
	{
		if (m_BaseAlign != 0)
			return;

		m_BaseAlign = GetUniformBaseAlign(m_Type);
		for (auto &s : m_StructArray)
		{
			s.FillMetaData();
		}
	}

	void UniformStructArrayLayout::CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_StructArray.empty())
			return;

		FillMetaData();

		for (uint32_t i = 0; i < m_StructArray.size(); i++)
		{
			auto &s = m_StructArray[i];
			s.CalculateOffset(byteUsed, offset, blockStart && (i == 0));
			byteUsed = s.GetByteUsed();
			offset = s.GetAlignOffset();

			if (i == 0)
				m_AlignOffset = s.GetAlignOffset();
		}

		auto &last = m_StructArray.back();
		uint32_t lastOccupy = last.GetOccupied();
		// NOTE: mat array has end padding.
		m_EndPadding = CalcAlignOffset(lastOccupy, m_BaseAlign);
		m_ByteUsed = m_EndPadding - m_AlignOffset;
	}

	std::string UniformStructArrayLayout::ToString() const
	{
		std::stringstream ss{};
		ss << "name[" << m_Name << "], base align[" << m_BaseAlign << "]\n";
		for (uint32_t i = 0; i < m_StructArray.size(); i++)
		{
			ss << m_StructArray[i].ToString() << ", ";
		}
		ss << "\nend padding[" << m_EndPadding << "]";
		return ss.str();
	}
}