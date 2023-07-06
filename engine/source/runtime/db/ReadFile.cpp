#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <glm/gtc/matrix_transform.hpp>
#include <climits>

#include "ReadFile.h"

#include "scenegraph/node/GeoNode.h"
#include "scenegraph/node/drawable/Geometry.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ReadFile"

namespace pio
{
	namespace db
	{
		const std::string ReadFile::ASSETS_ROOT_DIR = "assets/objects/";

		const std::string ReadFile::POSTFIX_OBJ = ".obj";

		std::shared_ptr<sgf::Node> ReadFile::readObj(const std::string &name)
		{
			std::string absPath = ASSETS_ROOT_DIR + name + "/" + name + POSTFIX_OBJ;

			Assimp::Importer importer;
			const aiScene *scene = importer.ReadFile(absPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				LOG_ERR("fail to load obj from [%s], reason[%s]", absPath.c_str(), importer.GetErrorString());
				return false;
			}
			auto root = std::shared_ptr<sgf::Node>(new sgf::GeoNode(name));
			processNode(scene, scene->mRootNode, name, root);

			LOG_DEBUG("finish loading obj[%s]", absPath.c_str());

			return root;
		}

		void ReadFile::processNode(const aiScene *scene, aiNode *node, const std::string &objName,
								   std::shared_ptr<sgf::Node> &parent)
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
				if (mesh)
				{
					processMesh(i, scene, mesh, objName, parent);
				}
			}

			// process the children for current node
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				auto child = std::shared_ptr<sgf::Node>(new sgf::GeoNode(node->mChildren[i]->mName.C_Str()));
				auto pGroup = parent->as<sgf::Group>();
				if (pGroup) { pGroup->addChild(child); }
				processNode(scene, node->mChildren[i], objName, child);
			}
		}

		void ReadFile::processMesh(int meshIndex, const aiScene *scene, aiMesh *mesh, const std::string &objName,
								   std::shared_ptr<sgf::Node> &parent)
		{
			std::shared_ptr<sgf::Node> node{ nullptr };
			sgf::Group *pParent{ nullptr };
			sgf::Geometry *pGeometry{ nullptr };

			pParent = parent->as<sgf::Group>();
			if (!pParent)
			{
				LOG_ERR("parent is not Group");
				return;
			}

			std::string childName = pParent->getName() + "_node" + std::to_string(pParent->childSize());
			node = std::shared_ptr<sgf::Node>(new sgf::Geometry(childName));
			if (!node || !(pGeometry = node->as<sgf::Geometry>()))
			{
				LOG_ERR("fail to malloc ptr for Geometry");
				return;
			}
			pParent->addChild(node);

			std::vector<sgf::Vertex> vertexArray{};
			std::vector<uint32_t>    indices{};
			glm::vec3 min{ FLT_MAX }, max{ FLT_MIN };

			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				sgf::Vertex vertex{};
				vertex.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
				vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
				vertex.tangents = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				min = glm::min(min, vertex.pos);
				max = glm::max(max, vertex.pos);
				if (mesh->mTextureCoords[0])
				{
					vertex.texCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
				}
				vertexArray.push_back(std::move(vertex));
			}
			pGeometry->setVertexArray(vertexArray);

			// TODO: add BoundingBox

			// every face represent a primitive
			// we use flag aiProcess_Triangulate to import the model, so a single primitive is a triangle
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (int j = 0; j < face.mNumIndices; ++j)
				{
					indices.push_back(face.mIndices[j]);
				}
			}
			pGeometry->setIndiceArray(indices);

			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial *mt = scene->mMaterials[mesh->mMaterialIndex];
				if (mt)
				{
					aiString texName;
					if (mt->GetTextureCount(aiTextureType_DIFFUSE) > 0)
					{
						mt->GetTexture(aiTextureType_DIFFUSE, 0, &texName);

						//TODO: add support for texture
						std::string srcPath = ASSETS_ROOT_DIR + objName + '/' + texName.C_Str();
					}

					if (mt->GetTextureCount(aiTextureType_SPECULAR) > 0)
					{
						mt->GetTexture(aiTextureType_SPECULAR, 0, &texName);

						std::string srcPath = ASSETS_ROOT_DIR + objName + '/' + texName.C_Str();
					}

					if (mt->GetTextureCount(aiTextureType_AMBIENT) > 0)
					{
						mt->GetTexture(aiTextureType_AMBIENT, 0, &texName);

						std::string srcPath = ASSETS_ROOT_DIR + objName + '/' + texName.C_Str();
					}

					// This texture is used for normal mapping.
					/*if (mt->GetTextureCount(aiTextureType_HEIGHT) > 0)
					{
						mt->GetTexture(aiTextureType_HEIGHT, 0, &texName);

						std::string srcPath = ASSETS_ROOT_DIR + objName + '/' + texName.C_Str();
					}*/

					aiColor3D color;
					ai_real   valFloat;
					aiString  valStr;
					ai_int    valInt;
					if (mt->Get(AI_MATKEY_NAME, valStr) == aiReturn_SUCCESS)
					{
						valStr.C_Str();
					}

					if (mt->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
					{
						glm::vec3(color.r, color.g, color.b);
					}

					if (mt->Get(AI_MATKEY_COLOR_SPECULAR, color) == aiReturn_SUCCESS)
					{
						glm::vec3(color.r, color.g, color.b);
					}

					if (mt->Get(AI_MATKEY_COLOR_AMBIENT, color) == aiReturn_SUCCESS)
					{
						glm::vec3(color.r, color.g, color.b);
					}

					if (mt->Get(AI_MATKEY_SHININESS, valFloat) == aiReturn_SUCCESS)
					{
						valFloat;
					}

					if (mt->Get(AI_MATKEY_SHADING_MODEL, valInt) == aiReturn_SUCCESS)
					{
						
					}
				}
			}
		}

	}
}