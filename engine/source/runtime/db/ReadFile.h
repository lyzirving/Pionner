#ifndef __PIONNER_DB_READ_FILE_H__
#define __PIONNER_DB_READ_FILE_H__

#include <string>
#include <memory>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace pio
{
	namespace sgf
	{
		class Node;
	}

	namespace db
	{
		class ReadFile
		{
		public:
			static std::shared_ptr<sgf::Node> readObj(const std::string &name);

		private:
			static void processNode(const aiScene *scene, aiNode *node, const std::string &objName,
									std::shared_ptr<sgf::Node> &parent);
			static void processMesh(int meshIndex, const aiScene *scene, aiMesh *mesh, const std::string &objName, 
									std::shared_ptr<sgf::Node> &parent);

		private:
			ReadFile();
			~ReadFile() = default;

		private:
			static const std::string ASSETS_ROOT_DIR;

			static const std::string POSTFIX_OBJ;
		};
	}
}

#endif	