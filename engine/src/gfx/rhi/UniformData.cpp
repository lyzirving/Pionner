#include "UniformData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformData"

namespace pio
{
	#define PIO_POOL_BUFF_CAP (1024 * 1024) //1MB

	PIO_SINGLETON_IMPL(UniformDataPool)

	UniformDataPool::UniformDataPool()
	{
		m_buffBlocks[0] = CreateBufferBlock();
	}

	UniformDataPool::~UniformDataPool()
	{
		for (size_t i = 0; i < k_PoolLimit; i++)
		{
			if (m_buffBlocks[i].Data)
			{
				std::free(m_buffBlocks[i].Data);
				m_buffBlocks[i].Data = nullptr;
			}
		}
	}

	void UniformDataPool::Init()
	{
		UniformDataPool::Get();
	}

	void UniformDataPool::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		delete s_instance;
		s_instance = nullptr;
	}

	Ref<UniformData> UniformDataPool::GetUniformData(UniformDataType type, const std::string& name)
	{
		auto data = CreateRef<UniformData>(type, name);
		auto byteUsed = data->m_byteUsed;
		if (byteUsed != 0)
		{
			uint32_t blockIdx = UniformDataPool::Get()->getValidBlock(byteUsed);
			auto& block = UniformDataPool::Get()->m_buffBlocks[blockIdx];
			uint32_t elemIdx = block.Elems.size();

			data->m_addr.BlockIdx = blockIdx;
			data->m_addr.ElemIdx = elemIdx;
			data->m_addr.StartOffset = block.Size;

			block.Size += byteUsed;
			block.Elems.push_back(data);

			CheckBlockCap(block);
		}
		return data;
	}

	uint32_t UniformDataPool::getValidBlock(uint32_t byteUse)
	{
		for (uint32_t i = 0; i < k_PoolLimit; i++)
		{
			if (m_buffBlocks[i].Size + byteUse <= m_buffBlocks[i].Cap)
				return i;
		}
		LOGE("err! out of memory for UniformDataPool");
		std::abort();
		return 0;
	}

	UniformDataPool::PoolBufferBlock UniformDataPool::CreateBufferBlock()
	{
		PoolBufferBlock block;
		block.Cap = PIO_POOL_BUFF_CAP;
		block.Size = 0;
		block.Data = std::calloc(PIO_POOL_BUFF_CAP, sizeof(unsigned char));
		return block;
	}

	void UniformDataPool::CheckBlockCap(PoolBufferBlock& block)
	{
		float ratio = float(block.Size) / float(block.Cap);
		if (ratio < 0.8f)
		{
			return;
		}
		LOGD("prepare to arrange PoolBufferBlock's data");
		std::set<uint32_t> deleteIdx{};
		auto& elems = block.Elems;

		//[TODO] find a more efficient strategy
		for (uint32_t i = 0; i < elems.size(); i++)
		{
			if (!UniformDataPool::bUsing(elems[i]))
			{
				deleteIdx.insert(i);
			}
		}

		if (!deleteIdx.empty())
		{
			for (uint32_t i = 0; i < elems.size(); i++)
			{
				auto it = deleteIdx.find(i);
				if (it != deleteIdx.end())
				{
					//TODO
					deleteIdx.erase(it);
				}
			}
		}
		LOGD("finish arranging PoolBufferBlock's data");
	}

	bool UniformDataPool::bUsing(Ref<UniformData>& data)
	{
		return data && data.use_count() >= 2;
	}

	UniformData::UniformData(UniformDataType type, const std::string& name) : m_type(type), m_name(name), m_byteUsed(Rhi::GetUniformByteSize(type, 1))
	{
	}

	UniformData::~UniformData()
	{
	}
}