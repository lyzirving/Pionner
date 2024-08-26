#ifndef __PIONNER_GFX_RHI_UNIFORM_DATA_H__
#define __PIONNER_GFX_RHI_UNIFORM_DATA_H__

#include "RhiDef.h"

namespace pio
{
	class UniformData;

	class UniformDataPool
	{
		PIO_SINGLETON_DECLARE(UniformDataPool)
	public:
		static Ref<UniformData> GetUniformData(UniformDataType type, const std::string& name);
	private:
		static constexpr uint32_t k_PoolLimit = 20;

		struct PoolBufferBlock
		{
			void* Data{ nullptr };
			uint32_t Cap{ 0 };      //Total capacity of buffer 
			uint32_t Size{ 0 };     //Byte size that has been used

			std::vector<Ref<UniformData>> Elems;
		};

		uint32_t getValidBlock(uint32_t byteUse);

	private:
		static PoolBufferBlock CreateBufferBlock();
		static void CheckBlockCap(PoolBufferBlock& block);
		static bool bUsing(Ref<UniformData>& data);

	private:
		PoolBufferBlock m_buffBlocks[k_PoolLimit];
	};

	class UniformData
	{
	public:
		UniformData(UniformDataType type, const std::string& name);
		~UniformData();

		UniformDataType type() const { return m_type; }
		const std::string name() const { return m_name; }
		uint32_t byteUsed() const { return m_byteUsed; }

	private:
		struct DataLocation
		{
			int32_t BlockIdx{ -1 };
			int32_t ElemIdx{ -1 };
			uint32_t StartOffset{ 0 };
		};

	private:
		UniformDataType m_type{ UniformDataType::None };
		std::string m_name{};
		uint32_t m_byteUsed{ 0 };
		DataLocation m_addr;

	private:
		friend class UniformDataPool;
	};
}

#endif