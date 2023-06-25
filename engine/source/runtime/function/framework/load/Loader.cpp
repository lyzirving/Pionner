#include <climits>
#include <string.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <glm/gtc/matrix_transform.hpp>

#include "function/framework/load/Loader.h"

#include "function/render/entity/RenderEntity.h"
#include "function/global/GlobalContext.h"
#include "function/render/RenderSystem.h"
#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/rhi/opengl/buffer/GLVertexBuffer.h"
#include "function/render/rhi/opengl/buffer/GLIndexBuffer.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Loader"

namespace Pionner
{
	Loader::Loader()
	{
	}

	Loader::~Loader() = default;

	bool Loader::load(const std::string &path, std::shared_ptr<RenderEntity> &root)
	{
		bool success{ false };

		if (path.empty())
			return success;

		int dot = std::string::npos;
		std::string fmt;

		dot = path.find_last_of('.');
		if (dot == std::string::npos)
		{
			LOG_ERR("invalid path[%s]", path.c_str());
			return success;
		}

		fmt = path.substr(dot + 1);

		if (std::strcmp(fmt.c_str(), "obj") == 0)
		{
			success = parseObj(path, root);
		}
		else
		{
			LOG_FATAL("unknown format[%s]", fmt.c_str());
		}

		return success;
	}

	bool Loader::parseObj(const std::string &path, std::shared_ptr<RenderEntity> &root)
	{
		int lastBackslash = path.find_last_of('/');
		int lastDot = path.find_last_of('.');

		std::string rootDir = path.substr(0, lastBackslash);
		std::string name = path.substr(lastBackslash + 1, lastDot - lastBackslash - 1);

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERR("fail to load obj from [%s], reason[%s]", path.c_str(), importer.GetErrorString());
			return false;
		}
		root->m_name = name;

		processNode(scene->mRootNode, scene, rootDir, root);

		LOG_DEBUG("finish loading obj[%s]", path.c_str());
		return true;
	}

	void Loader::processNode(aiNode *node, const aiScene *scene, const std::string &rootDir, std::shared_ptr<RenderEntity> &entity)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh != nullptr)
			{
				processMesh(i, mesh, scene, rootDir, entity);
			}
		}

		// process the children for current node
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			auto child = RenderEntity::makeEntity(entity->m_type);
			child->m_parent = entity;
			child->m_name = node->mChildren[i]->mName.C_Str();
			entity->m_children.push_back(child);
			processNode(node->mChildren[i], scene, rootDir, child);
		}
	}

	void Loader::processMesh(int meshIndex, aiMesh *mesh, const aiScene *scene, const std::string &rootDir,
							 std::shared_ptr<RenderEntity> &entity)
	{
		glm::vec3 min{ FLT_MAX }, max{ FLT_MIN };
		std::vector<Vertex> vertexArray{};
		std::vector<uint32_t> indices{};
		std::shared_ptr<GfxBuffer> vBuffer{ nullptr }, indBuffer{ nullptr }, texture{ nullptr };

		std::shared_ptr<EntityPart> part = std::shared_ptr<EntityPart>(new EntityPart);
		part->m_owner = entity;

		std::shared_ptr<RenderResourceMgr> resource = g_runtimeCtx.m_renderSystem->getResourceMgr();
		vBuffer = resource->allocate(BUF_VERTEX);
		indBuffer = resource->allocate(BUF_INDICE);

		part->m_partIndex = meshIndex;
		part->m_vertexSlot = vBuffer->getSlot();
		part->m_indicesSlot = indBuffer->getSlot();

		// parse vertex array
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex{};
			vertex.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			min = glm::min(min, vertex.pos);
			max = glm::max(max, vertex.pos);
			if (mesh->mTextureCoords[0])
			{
				vertex.texCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			vertexArray.push_back(std::move(vertex));
		}

		if (!vertexArray.empty())
		{
			vBuffer->insertData<Vertex>(vertexArray);
		}

		part->m_aabb.setAA(min);
		part->m_aabb.setBB(max);

		// process the indices
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
		indBuffer->insertData<uint32_t>(indices);

		// every mesh only has one GfxMaterial
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial *mt = scene->mMaterials[mesh->mMaterialIndex];
			if (mt)
			{
				aiString texName;
				bool found{ false };
				if (mt->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				{
					mt->GetTexture(aiTextureType_DIFFUSE, 0, &texName);

					std::string srcPath = rootDir + '/' + texName.C_Str();
					texture = resource->allocate(BUF_TEXTURE);
					texture->insertData(srcPath);
					texture->loadRawData();
					part->m_material.m_diffSlot = texture->getSlot();
				}

				if (mt->GetTextureCount(aiTextureType_SPECULAR) > 0)
				{
					mt->GetTexture(aiTextureType_SPECULAR, 0, &texName);

					std::string srcPath = rootDir + '/' + texName.C_Str();
					texture = resource->allocate(BUF_TEXTURE);
					texture->insertData(srcPath);
					texture->loadRawData();
					part->m_material.m_specSlot = texture->getSlot();
				}
				/*if (mt->GetTextureCount(aiTextureType_AMBIENT) > 0)
				{
					mt->GetTexture(aiTextureType_AMBIENT, 0, &texName);
					part->m_material.m_type = MAT_AMBIENT;
					found = true;
				}*/

				aiColor3D color;
				ai_real   valFloat;
				aiString  valStr;
				ai_int    valInt;
				if (mt->Get(AI_MATKEY_NAME, valStr) == aiReturn_SUCCESS)
				{
					part->m_material.m_name = valStr.C_Str();
				}

				if (mt->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
				{
					part->m_material.m_colorDiffuse = glm::vec3(color.r, color.g, color.b);
				}

				if (mt->Get(AI_MATKEY_COLOR_SPECULAR, color) == aiReturn_SUCCESS)
				{
					part->m_material.m_colorSpecular = glm::vec3(color.r, color.g, color.b);
				}

				if (mt->Get(AI_MATKEY_COLOR_AMBIENT, color) == aiReturn_SUCCESS)
				{
					part->m_material.m_colorAmbient = glm::vec3(color.r, color.g, color.b);
				}

				if (mt->Get(AI_MATKEY_SHININESS, valFloat) == aiReturn_SUCCESS)
				{
					part->m_material.m_shiness = valFloat;
				}

				if (mt->Get(AI_MATKEY_SHADING_MODEL, valInt) == aiReturn_SUCCESS)
				{
					if (valInt == MODE_CONSTANT || valInt == MODE_DIFFUSE || valInt == MODE_DIFFUSE_SPECULAR)
					{
						part->m_material.m_mode = ShadingMode(valInt);
					}
				}
			}
		}

		// insert mesh as part into the entity
		entity->m_parts.push_back(std::move(part));
	}
}