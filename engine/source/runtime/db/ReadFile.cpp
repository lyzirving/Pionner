#include "ReadFile.h"

#include "scenegraph/node/GeoNode.h"

namespace pio
{
	namespace db
	{
		const std::string ReadFile::ASSETS_ROOT_DIR = "assets/objects/spiderman/";

		std::shared_ptr<sgf::GeoNode> ReadFile::readObj(const std::string &name)
		{
			//TODO:
			return std::shared_ptr<sgf::GeoNode>();
		}
	}
}