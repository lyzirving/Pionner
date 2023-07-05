#ifndef __PIONNER_DB_READ_FILE_H__
#define __PIONNER_DB_READ_FILE_H__

#include <string>
#include <memory>

namespace pio
{
	namespace sgf
	{
		class GeoNode;
	}

	namespace db
	{
		class ReadFile
		{
		public:
			static std::shared_ptr<sgf::GeoNode> readObj(const std::string &name);

		private:
			ReadFile();
			~ReadFile() = default;

		private:
			static const std::string ASSETS_ROOT_DIR;
		};
	}
}

#endif	