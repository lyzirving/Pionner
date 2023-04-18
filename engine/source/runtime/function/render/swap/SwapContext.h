#ifndef __SWAP_CONTEXT_H__
#define __SWAP_CONTEXT_H__

#include <vector>
#include <memory>
#include <bitset>

namespace Pionner
{
	class RenderEntity;

	enum SwapDataSource : uint8_t
	{
		SOURCE_OBJ_ENTITY,
		SWAP_SOURCE_COUNT
	};

	enum SwapDataType : uint8_t
	{
		TYPE_LOGIC = 0,
		TYPE_RENDER,
		SWAP_TYPE_COUNT
	};

	struct SwapData
	{
		std::bitset<SWAP_SOURCE_COUNT>             m_changeFlag;
		std::vector<std::shared_ptr<RenderEntity>> m_objEntities;

		SwapData();

		void addObjEntities(std::vector<std::shared_ptr<RenderEntity>> &entities);

		void clear();
		void clearEntity();
	};

	class SwapContext
	{
	public:
		SwapContext();
		~SwapContext();

		bool needSwap();
		void swapData();

		SwapData &logicData();
		SwapData &renderData();

	public:
		SwapData     m_data[2];
		SwapDataType m_logicInd;
		SwapDataType m_renderInd;
	};
}

#endif