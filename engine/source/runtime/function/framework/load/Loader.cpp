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

	std::shared_ptr<RenderEntity> Loader::load(const std::string &path)
	{
		std::shared_ptr<RenderEntity> entity{ nullptr };

		if (path.empty())
			return entity;

		int dot = std::string::npos;
		std::string fmt;

		dot = path.find_last_of('.');
		if (dot == std::string::npos)
		{
			LOG_ERR("invalid path[%s]", path.c_str());
			return entity;
		}
		fmt = path.substr(dot + 1);

		if (std::strcmp(fmt.c_str(), "obj") == 0)
		{
			entity = parseObj(path);
		}
		else
		{
			LOG_FATAL("unknown format[%s]", fmt.c_str());
		}
		return entity;
	}

	std::shared_ptr<RenderEntity> Loader::parseObj(const std::string &path)
	{
		std::shared_ptr<RenderEntity> root{ nullptr };

		std::string rootDir = path.substr(0, path.find_last_of('/'));

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERR("fail to load obj from [%s], reason[%s]", path.c_str(), importer.GetErrorString());
			return root;
		}

		root = std::shared_ptr<RenderEntity>(new RenderEntity);
		root->m_type = ENTITY_MODEL;

		processNode(scene->mRootNode, scene, rootDir, root);

		LOG_DEBUG("finish loading obj[%s]", path.c_str());

		return root;
	}

	void Loader::processNode(aiNode *node, const aiScene *scene, const std::string &rootDir,
							 std::shared_ptr<RenderEntity> &entity)
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
			std::shared_ptr<RenderEntity> child = std::shared_ptr<RenderEntity>(new RenderEntity);
			child->m_type = entity->m_type;
			child->m_parent = entity;
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
		vBuffer = resource->allocate(BUF_MEM_ARRAY);
		indBuffer = resource->allocate(BUF_EBO);

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
					part->m_material.m_type = MAT_DIFFUSE;
					found = true;
				}
				else if (mt->GetTextureCount(aiTextureType_SPECULAR) > 0)
				{
					mt->GetTexture(aiTextureType_SPECULAR, 0, &texName);
					part->m_material.m_type = MAT_SPECULAR;
					found = true;
				}
				else if (mt->GetTextureCount(aiTextureType_AMBIENT) > 0)
				{
					mt->GetTexture(aiTextureType_AMBIENT, 0, &texName);
					part->m_material.m_type = MAT_AMBIENT;
					found = true;
				}

				if (found)
				{
					std::string srcPath = rootDir + '/' + texName.C_Str();

					texture = resource->allocate(BUF_TEXTURE);
					texture->insertData(srcPath);
					texture->loadRawData();

					part->m_material.m_slot = texture->getSlot();
				}

				aiColor3D color;
				ai_real val;
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
				if (mt->Get(AI_MATKEY_SHININESS, val) == aiReturn_SUCCESS)
				{
					part->m_material.m_shiness = val;
				}
			}
		}

		// insert mesh as part into the entity
		entity->m_parts.push_back(std::move(part));
	}
}