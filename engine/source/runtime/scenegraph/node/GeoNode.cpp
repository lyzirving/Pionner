#include "GeoNode.h"

namespace pio
{
	namespace sgf
	{
		GeoNode::GeoNode() : Group()
		{
		}

		GeoNode::GeoNode(const std::string &name) : Group(name)
		{
		}

		GeoNode::~GeoNode()
		{
		}
	}
}