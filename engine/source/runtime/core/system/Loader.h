#ifndef __PIONNER_LOADER_H__
#define __PIONNER_LOADER_H__

#include <string>
#include <memory>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace pio
{
	class RenderEntity;

	class Loader
	{
	public:
		static bool load(const std::string &path, std::shared_ptr<RenderEntity> &root);

	private:
		static bool parseObj(const std::string &path, std::shared_ptr<RenderEntity> &root);
		static void processNode(aiNode *node, const aiScene *scene, const std::string &rootDir, std::shared_ptr<RenderEntity> &entity);

		static void processMesh(int meshIndex, aiMesh *mesh, const aiScene *scene,
								const std::string &rootDir, std::shared_ptr<RenderEntity> &entity);

		Loader();
		~Loader();
	};
}

#endif