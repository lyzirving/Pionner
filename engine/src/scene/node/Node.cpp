#include "Node.h"

namespace pio
{
	uint32_t Node::k_NodeNum = 0;	

	Node::Node(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name) 
		: m_key(key), m_regi(regi), m_uuid(), m_context(context), m_name(name)
	{
		k_NodeNum++;
	}
}