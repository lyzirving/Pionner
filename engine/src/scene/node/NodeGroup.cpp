#include "NodeGroup.h"

namespace pio
{
	bool NodeGroup::GroupInsert(const Ref<Node>& node)
	{
		int32_t idx = InsertChild(node);
		if(idx >= 0)
		{
			OnGroupSizeChanged();
			node->SetGroupIndex(idx);
			m_Children[idx]->OnAttachGroup();			
			return true;
		}
		return false;
	}

	bool NodeGroup::GroupRemoval(const Ref<Node>& node)
	{
		int32_t idx = RemoveChild(node);
		if(idx >= 0)
		{
			OnGroupSizeChanged();
			node->OnDetachGroup();
			for(int32_t i = idx; i < m_Children.size(); i++)
				m_Children[i]->SetGroupIndex(i);			
			return true;
		}
		return false;
	}

	Ref<Node> NodeGroup::PopFront()
	{
		if(m_Children.empty())
			return Ref<Node>();

		Ref<Node> result;
		if(m_Children.size() == 1)
		{
			result = m_Children.back();
			m_Children.pop_back();
			OnGroupSizeChanged();
			result->OnDetachGroup();
			return result;
		}

		std::swap(m_Children[0], m_Children[m_Children.size() - 1]);
		result = m_Children.back();
		m_Children.pop_back();
		OnGroupSizeChanged();
		result->OnDetachGroup();
		m_Children[0]->SetGroupIndex(0);		
		return result;
	}
}