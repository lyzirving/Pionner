#include "UniformPack.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformPack"

namespace pio
{
	// ------------------------ UniformPack -------------------------
	// --------------------------------------------------------------
	bool UniformPack::IsPrimitive(UniformDataType type)
	{
		switch (type)
		{
			case UniformDataType::Bool:
			case UniformDataType::Float:
			case UniformDataType::Int:
			case UniformDataType::UInt:
			case UniformDataType::Vec2:
			case UniformDataType::Vec3:
			case UniformDataType::Vec4:
			case UniformDataType::IVec2:
			case UniformDataType::IVec3:
			case UniformDataType::IVec4:
			case UniformDataType::UVec2:
			case UniformDataType::UVec3:
			case UniformDataType::UVec4:
				return true;
			default:
				return false;;
		}
	}

	uint32_t UniformPack::CalcAlignOffset(uint32_t occupied, uint32_t baseAlign)
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

	UniformPack::UniformPack()
	{
	}

	UniformPack::UniformPack(UniformDataType type) : m_type(type)
	{
	}

	UniformPack::UniformPack(UniformDataType type, const std::string &name) : m_type(type), m_name(name)
	{
	}

	void UniformPack::fillMetaData()
	{
		if (m_baseAlign != 0)
			return;

		m_baseAlign = Rhi::GetUniformBaseAlign(m_type);
		m_byteUsed = Rhi::GetUniformColumnsByteSize(m_type);
	}

	void UniformPack::calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		fillMetaData();
		m_alignOffset = blockStart ? 0 : CalcAlignOffset(byteUsed + offset, m_baseAlign);
	}

	std::string UniformPack::toString()
	{
		std::stringstream ss{};
		ss << "name[" << m_name << "], base align[" << m_baseAlign << "], align offset[" << m_alignOffset << "], byte used[" << m_byteUsed << "]";
		return ss.str();
	}

	// ------------------------ UnimPackMat --------------------------
	// --------------------------------------------------------------
	UniformDataType UnimPackMat::GetColumnDataType(UniformDataType type)
	{
		switch (type)
		{
			case UniformDataType::Mat2:
				return UniformDataType::Vec2;
			case UniformDataType::Mat3:
				return UniformDataType::Vec3;
			case UniformDataType::Mat4:
				return UniformDataType::Vec4;
			default:
				LOGE("UnimPackMat: invalid Mat type[%u]", type);
				std::abort();
				return UniformDataType::Vec2;
		}
	}

	uint32_t UnimPackMat::GetMatColumnNum(UniformDataType type)
	{
		switch (type)
		{
			case UniformDataType::Mat2:
				return 2;
			case UniformDataType::Mat3:
				return 3;
			case UniformDataType::Mat4:
				return 4;
			default:
				LOGE("UnimPackMat: invalid Mat type[%u]", type);
				std::abort();
				return 0;
		}
	}

	UnimPackMat::UnimPackMat() : UniformPack()
	{
	}

	UnimPackMat::UnimPackMat(UniformDataType type, uint32_t columnNum, const std::string &name)
		: UniformPack(type, name), m_columnNum(columnNum)
	{
		if (m_columnNum <= 1)
		{
			//LOGE("input column num[%u] is invald!", m_columnNum);
			return;
		}
		fillMetaData();
	}

	UnimPackMat::~UnimPackMat() = default;

	void UnimPackMat::fillMetaData()
	{
		if (m_baseAlign != 0)
			return;

		m_columns.resize(m_columnNum);
		m_baseAlign = Rhi::GetUniformBaseAlign(m_type);
		for (uint32_t i = 0; i < m_columnNum; i++)
		{
			m_columns[i].m_type = UnimPackMat::GetColumnDataType(m_type);
			m_columns[i].m_baseAlign = m_baseAlign;
			m_columns[i].m_byteUsed = Rhi::GetUniformColumnsByteSize(m_columns[i].m_type);
		}
	}

	void UnimPackMat::calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_columnNum <= 1)
		{
			LOGE("invalid column num[%u]!", m_columnNum);
			return;
		}

		fillMetaData();

		for (uint32_t i = 0; i < m_columns.size(); i++)
		{
			auto &col = m_columns[i];
			col.calculateOffset(byteUsed, offset, blockStart && (i == 0));
			byteUsed = col.getByteUsed();
			offset = col.getAlignOffset();

			if (i == 0)
				m_alignOffset = col.getAlignOffset();
		}

		auto &last = m_columns.back();
		uint32_t lastOccupy = last.getOccupied();
		// NOTE: matrix has end padding.
		m_endPadding = CalcAlignOffset(lastOccupy, m_baseAlign);
		m_byteUsed = m_endPadding - m_alignOffset;
	}

	std::string UnimPackMat::toString()
	{
		std::stringstream ss{};
		ss << "name[" << m_name << "], base align[" << m_baseAlign << "]\n";
		for (uint32_t i = 0; i < m_columns.size(); i++)
		{
			ss << "column[" << i << "], align offset[" << m_columns[i].getAlignOffset() << "], byte used[" << m_columns[i].getByteUsed() << "], ";
		}
		ss << "\nending padding[" << m_endPadding << "]";
		return ss.str();
	}

	// ------------------------ UnimPackArray --------------------------
	// ----------------------------------------------------------------

	UniformDataType UnimPackArray::GetArrayDataType(UniformDataType type)
	{
		switch (type)
		{
			case UniformDataType::FloatArray:
				return UniformDataType::Float;
			case UniformDataType::IntArray:
				return UniformDataType::Int;
			case UniformDataType::UIntArray:
				return UniformDataType::UInt;
			default:
				LOGE("UnimPackMat: invalid array type[%u]", type);
				std::abort();
				return UniformDataType::Float;
		}
	}

	UnimPackArray::UnimPackArray(UniformDataType type, uint32_t arrayNum, const std::string &name)
		: UniformPack(type, name), m_arrayNum(arrayNum)
	{
		fillMetaData();
	}

	UnimPackArray::~UnimPackArray() = default;

	void UnimPackArray::fillMetaData()
	{
		if (m_baseAlign != 0)
			return;

		m_array.resize(m_arrayNum);
		m_baseAlign = Rhi::GetUniformBaseAlign(m_type);
		for (uint32_t i = 0; i < m_arrayNum; i++)
		{
			m_array[i].m_type = UnimPackArray::GetArrayDataType(m_type);
			m_array[i].m_baseAlign = m_baseAlign;
			m_array[i].m_byteUsed = Rhi::GetUniformColumnsByteSize(m_array[i].m_type);
		}
	}

	void UnimPackArray::calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_arrayNum < 1)
		{
			LOGE("invalid array num[%u]", m_arrayNum);
			return;
		}

		fillMetaData();

		for (uint32_t i = 0; i < m_array.size(); i++)
		{
			auto &item = m_array[i];
			item.calculateOffset(byteUsed, offset, blockStart && (i == 0));
			byteUsed = item.getByteUsed();
			offset = item.getAlignOffset();

			if (i == 0)
				m_alignOffset = item.getAlignOffset();
		}

		auto &last = m_array.back();
		uint32_t lastOccupy = last.getOccupied();
		// NOTE: array has end padding.
		m_endPadding = CalcAlignOffset(lastOccupy, m_baseAlign);
		m_byteUsed = m_endPadding - m_alignOffset;
	}

	std::string UnimPackArray::toString()
	{
		std::stringstream ss{};
		ss << "name[" << m_name << "], array base align[" << m_baseAlign << "]\n";
		for (uint32_t i = 0; i < m_array.size(); i++)
		{
			ss << "item[" << i << "], align offset[" << m_array[i].getAlignOffset()
				<< "], byte used[" << m_array[i].getByteUsed() << "], ";
		}
		ss << "\nend padding[" << m_endPadding << "]";
		return ss.str();
	}
	// ------------------------ UnimPackMatArray --------------------------
	// -------------------------------------------------------------------

	UniformDataType UnimPackMatArray::GetArrayDataType(UniformDataType type)
	{
		switch (type)
		{
			case UniformDataType::Mat2Array:
				return UniformDataType::Mat2;
			case UniformDataType::Mat3Array:
				return UniformDataType::Mat3;
			case UniformDataType::Mat4Array:
				return UniformDataType::Mat4;
			default:
				LOGE("invalid mat array type[%]", type);
				std::abort();
				return UniformDataType::Mat2;
		}
	}

	UnimPackMatArray::UnimPackMatArray(UniformDataType type, uint32_t arrayNum, const std::string &name)
		: UniformPack(type, name), m_arrayNum(arrayNum)
	{
		fillMetaData();
	}

	UnimPackMatArray::~UnimPackMatArray() = default;

	void UnimPackMatArray::fillMetaData()
	{
		if (m_baseAlign != 0)
			return;

		m_matArray.resize(m_arrayNum);
		m_baseAlign = Rhi::GetUniformBaseAlign(m_type);
		for (uint32_t i = 0; i < m_arrayNum; i++)
		{
			m_matArray[i].m_type = UnimPackMatArray::GetArrayDataType(m_type);
			m_matArray[i].m_columnNum = UnimPackMat::GetMatColumnNum(m_matArray[i].m_type);
		}
	}

	void UnimPackMatArray::calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_matArray.empty())
			return;

		fillMetaData();

		for (uint32_t i = 0; i < m_matArray.size(); i++)
		{
			auto &mat = m_matArray[i];
			mat.calculateOffset(byteUsed, offset, blockStart && (i == 0));
			byteUsed = mat.getByteUsed();
			offset = mat.getAlignOffset();

			if (i == 0)
				m_alignOffset = mat.getAlignOffset();
		}

		auto &last = m_matArray.back();
		uint32_t lastOccupy = last.getOccupied();
		// NOTE: mat array has end padding.
		m_endPadding = CalcAlignOffset(lastOccupy, m_baseAlign);
		m_byteUsed = m_endPadding - m_alignOffset;
	}

	std::string UnimPackMatArray::toString()
	{
		std::stringstream ss{};
		ss << "name[" << m_name <<"], base align[" << m_baseAlign << "]\n";
		for (uint32_t i = 0; i < m_matArray.size(); i++)
		{
			ss << "item[" << i << "], align offset[" << m_matArray[i].getAlignOffset() 
			   << "], byte used[" << m_matArray[i].getByteUsed() << "](include padding), ";
		}
		ss << "\nend padding[" << m_endPadding << "]";
		return ss.str();
	}
	// ------------------------- UnimPackStruct ---------------------------
	// -------------------------------------------------------------------
	UnimPackStruct::UnimPackStruct() : UniformPack(UniformDataType::Struct)
	{
	}

	UnimPackStruct::UnimPackStruct(const std::string &name) : UniformPack(UniformDataType::Struct, name)
	{
	}

	UnimPackStruct::~UnimPackStruct()
	{
		auto it = m_data.begin();
		while (it != m_data.end())
		{
			it->second.reset();
			it = m_data.erase(it);
		}
	}

	void UnimPackStruct::fillMetaData()
	{
		if (m_baseAlign != 0 || m_data.empty())
			return;

		m_baseAlign = Rhi::GetUniformBaseAlign(m_type);
		for (auto &d : m_data)
		{
			d.second->fillMetaData();
		}
	}

	void UnimPackStruct::calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_data.empty())
			return;

		fillMetaData();

		if (blockStart)
			m_startPadding = 0;
		else
			m_startPadding = UniformPack::CalcAlignOffset(byteUsed + offset, m_baseAlign);

		uint32_t tmpOffset = m_alignOffset = m_startPadding;
		uint32_t tmpUsed = 0;
		Ref<UniformPack> last{};
		auto it = m_data.begin();
		while (it != m_data.end())
		{
			auto &d = it->second;
			// block start is alreay used
			d->calculateOffset(tmpUsed, tmpOffset, false);
			tmpUsed = d->getByteUsed();
			tmpOffset = d->getAlignOffset();
			last = d;
			it++;
		}

		uint32_t lastOccupy = last->getOccupied();
		m_endPadding = CalcAlignOffset(lastOccupy, m_baseAlign);
		m_byteUsed = m_endPadding - m_alignOffset;
	}

	std::string UnimPackStruct::toString()
	{
		std::stringstream ss{};
		ss << "name["<< m_name << "], base align[" << m_baseAlign << "], start padding[" << m_startPadding << "]\n";
		auto it = m_data.begin();
		while (it != m_data.end())
		{
			ss << "[" << it->first << "], base align[" << it->second->getBaseAlign()
				<< "], align offset[" << it->second->getAlignOffset()
				<< "], byte used[" << it->second->getByteUsed() << "], ";
			it++;
		}
		ss << "\nend padding[" << m_endPadding << "]";
		return ss.str();
	}

	// ------------------------- UnimPackStructArray ---------------------------
	// ------------------------------------------------------------------------

	UnimPackStructArray::UnimPackStructArray(const std::string &name)
		: UniformPack(UniformDataType::StructArray, name)
	{
	}

	UnimPackStructArray::~UnimPackStructArray() = default;

	void UnimPackStructArray::fillMetaData()
	{
		if (m_baseAlign != 0)
			return;

		m_baseAlign = Rhi::GetUniformBaseAlign(m_type);
		for (auto &s : m_structArray)
		{
			s.fillMetaData();
		}
	}

	void UnimPackStructArray::calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart)
	{
		if (m_structArray.empty())
			return;

		fillMetaData();

		for (uint32_t i = 0; i < m_structArray.size(); i++)
		{
			auto &s = m_structArray[i];
			s.calculateOffset(byteUsed, offset, blockStart && (i == 0));
			byteUsed = s.getByteUsed();
			offset = s.getAlignOffset();

			if (i == 0)
				m_alignOffset = s.getAlignOffset();
		}

		auto &last = m_structArray.back();
		uint32_t lastOccupy = last.getOccupied();
		// NOTE: mat array has end padding.
		m_endPadding = CalcAlignOffset(lastOccupy, m_baseAlign);
		m_byteUsed = m_endPadding - m_alignOffset;
	}

	std::string UnimPackStructArray::toString()
	{
		std::stringstream ss{};
		ss << "name[" << m_name << "], base align[" << m_baseAlign << "]\n";
		for (uint32_t i = 0; i < m_structArray.size(); i++)
		{
			ss << m_structArray[i].toString() << ", ";
		}
		ss << "\nend padding[" << m_endPadding << "]";
		return ss.str();
	}

	UniformBlock::UniformBlock()
	{
	}

	UniformBlock::~UniformBlock() = default;

	void UniformBlock::calculate()
	{
		uint32_t offset{ 0 };
		uint32_t byteUsed{ 0 };
		for (size_t i = 0; i < m_blockItems.size(); i++)
		{
			auto &item = m_blockItems[i];
			item->calculateOffset(byteUsed, offset, (i == 0));
			offset = item->getAlignOffset();
			byteUsed = item->getByteUsed();
			if (i == 0)
				m_alignOffset = offset;
			
			// [Note]: in some case, i can be both the first one and last one
			if (i == m_blockItems.size() - 1)
				m_byteUsed = offset + byteUsed - m_alignOffset;
		}
		m_buffer = CreateRef<Buffer>();
		m_buffer->allocate(0, m_byteUsed);
	}

	std::string UniformBlock::toString()
	{
		std::stringstream ss;
		ss << "\n---------- block align offset[" << m_alignOffset << "], byte used[" << m_byteUsed << "] ----------";
		for (size_t i = 0; i < m_blockItems.size(); i++)
		{
			ss << "\n";
			ss << m_blockItems[i]->toString();
		}
		return ss.str();
	}

	Ref<UniformPack> UniformBlock::CreateData(UniformDataType type, const std::string &name, uint32_t arrayNum)
	{
		switch (type)
		{
			case UniformDataType::Bool:
			case UniformDataType::Float:
			case UniformDataType::Int:
			case UniformDataType::UInt:
			case UniformDataType::Vec2:
			case UniformDataType::Vec3:
			case UniformDataType::Vec4:
			case UniformDataType::IVec2:
			case UniformDataType::IVec3:
			case UniformDataType::IVec4:
			case UniformDataType::UVec2:
			case UniformDataType::UVec3:
			case UniformDataType::UVec4:
				return CreateRef<UniformPack>(type, name);
			case UniformDataType::Mat2:
				return CreateRef<UniformPack, UnimPackMat>(type, 2, name);
			case UniformDataType::Mat3:
				return CreateRef<UniformPack, UnimPackMat>(type, 3, name);
			case UniformDataType::Mat4:
				return CreateRef<UniformPack, UnimPackMat>(type, 4, name);
			case UniformDataType::FloatArray:
			case UniformDataType::IntArray:
			case UniformDataType::UIntArray:
			case UniformDataType::Vec2Array:
			case UniformDataType::Vec3Array:
			case UniformDataType::Vec4Array:
				return CreateRef<UniformPack, UnimPackArray>(type, arrayNum, name);
			case UniformDataType::Mat2Array:
			case UniformDataType::Mat3Array:
			case UniformDataType::Mat4Array:
				return CreateRef<UniformPack, UnimPackMatArray>(type, arrayNum, name);
			case UniformDataType::Struct:
				return CreateRef<UniformPack, UnimPackStruct>(name);
			case UniformDataType::StructArray:
				return CreateRef<UniformPack, UnimPackStructArray>(name);
			default:
				LOGE("err! invalid data type[%u]", type);
				std::abort();
				return Ref<UniformPack>();
		}
	}

	template<>
	bool UniformPack::is<UnimPackMatArray>() 
	{
		return m_type == UniformDataType::Mat2Array ||
			   m_type == UniformDataType::Mat3Array ||
			   m_type == UniformDataType::Mat4Array;
	}

	template<>
	bool UniformPack::is<UnimPackStruct>() { return m_type == UniformDataType::Struct; }

	template<>
	bool UniformPack::is<UnimPackStructArray>() { return m_type == UniformDataType::StructArray; }
}