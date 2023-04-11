#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <string.h>

#include "function/framework/components/mesh/LoadMeshJob.h"
#include "function/framework/object/GameObjectDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LoadMeshJob"

namespace Pionner
{
	LoadMeshJob::LoadMeshJob(JobType type, JobObserver* ob)
		: Job(type, ob)
		, m_meshToLoad()
	{
	}

	LoadMeshJob::~LoadMeshJob()
	{
		m_ob = nullptr;
	}

	void LoadMeshJob::work(JobResult& result)
	{
		for (auto& item : m_meshToLoad)
		{
			// parse obj file
			if (!item.m_objDesc.m_srcPath.empty())
			{
				parseObj(item.m_objDesc.m_srcPath);
			}
		}
	}

	void LoadMeshJob::parseObj(const std::string& path)
	{
		int dotPos = path.find_last_of('.');
		if (dotPos == std::string::npos)
		{
			LOG_ERR("invalid pos after dot");
			return;
		}
		std::string fmt = path.substr(dotPos + 1);
		if (std::strcmp(fmt.c_str(), "obj") != 0)
		{
			LOG_ERR("file fmt is not obj");
			return;
		}

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate/*| aiProcess_FlipUVs*/);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			LOG_ERR("fail to load obj from [%s], reason[%s]", path.c_str(), importer.GetErrorString());
			return;
		}
		LOG_DEBUG("start to load obj from[%s]", path.c_str());
		processNode(scene->mRootNode, scene);
		LOG_DEBUG("finish loading obj[%s]", path.c_str());
	}

	void LoadMeshJob::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh != nullptr)
			{
				processMesh(mesh, scene);
			}
		}

		// process the children for current node
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void LoadMeshJob::processMesh(aiMesh* mesh, const aiScene* scene)
	{
	}

}