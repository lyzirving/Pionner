#include "NodeFactory.h"

namespace pio
{
	namespace scenegrf
	{
		std::shared_ptr<Node> NodeFactory::create(PioEntityType type)
		{
			return std::shared_ptr<Node>();
		}
	}
}