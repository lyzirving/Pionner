#include "NodeFactory.h"

#include "db/ReadFile.h" 
#include "drawable/Geometry.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "NodeFactory"

namespace pio
{
	namespace sgf
	{
		std::shared_ptr<Node> NodeFactory::create(const EntityParam &param)
		{
			switch (param.type)
			{
				case PIO_ENTITY_GEO:
				{
					if (param.nodeName.empty())
					{
						LOG_ERR("empty src name for Geo entity");
						break;
					}
					auto result = db::ReadFile::readObj(param.nodeName);
					return result;
				}
				default:
					break;
			}
			return std::shared_ptr<Node>();
		}
	}
}