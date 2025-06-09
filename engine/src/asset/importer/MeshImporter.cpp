#include "MeshImporter.h"

#include "asset/AssetMgr.h"
#include "asset/mesh/StaticMesh.h"
#include "asset/mesh/SkinnedMesh.h"
#include "asset/mesh/Skeleton.h"
#include "asset/mesh/AnimationClip.h"
#include "asset/material/Material.h"

#include "base/utils/AssimpUtils.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MeshImporter"

namespace pio
{
	void NormalizeBoneInfluence(BoneInfluence& influence)
	{
		float sum{ 0.f };
		for(size_t i = 0; i < PIO_MAX_BONE_INFLUENCE; i++)
		{
			if(influence.Indices[i] < 0)
				continue;
			sum += influence.Weights[i];
		}

		if(Math::IsZero(sum))
			return;

		for(size_t i = 0; i < PIO_MAX_BONE_INFLUENCE; i++)
		{
			if(influence.Indices[i] < 0)
				continue;
			influence.Weights[i] /= sum;
		}
	}

	std::vector<ScaleKey> SplitScales(aiNodeAnim* channel)
	{
		std::vector<ScaleKey> scaleKeys{};
		for(int i = 0; i < channel->mNumScalingKeys; ++i)
		{
			aiVector3D scale = channel->mScalingKeys[i].mValue;
			float timeStamp = channel->mScalingKeys[i].mTime;
			scaleKeys.emplace_back(timeStamp, AssimpUtils::Vec3FromAiVector3D(scale));
		}
		if(!scaleKeys.empty())
		{
			std::sort(scaleKeys.begin(), scaleKeys.end(), AnimationKeySort<ScaleKey>);
		}
		return scaleKeys;
	}

	std::vector<RotationKey> SplitRotations(aiNodeAnim* channel)
	{
		std::vector<RotationKey> rotationKeys{};
		for(int i = 0; i < channel->mNumRotationKeys; ++i)
		{
			aiQuaternion aiOrientation = channel->mRotationKeys[i].mValue;
			float timeStamp = channel->mRotationKeys[i].mTime;
			rotationKeys.emplace_back(timeStamp, AssimpUtils::QuatFromAiQuat(aiOrientation));
		}
		if(!rotationKeys.empty())
		{
			std::sort(rotationKeys.begin(), rotationKeys.end(), AnimationKeySort<RotationKey>);
		}
		return rotationKeys;
	}

	std::vector<TranslationKey> SplitTranslations(aiNodeAnim* channel)
	{
		std::vector<TranslationKey> translationKeys;
		for(int i = 0; i < channel->mNumPositionKeys; ++i)
		{
			aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
			float timeStamp = channel->mPositionKeys[i].mTime;
			translationKeys.emplace_back(timeStamp, AssimpUtils::Vec3FromAiVector3D(aiPosition));
		}
		if(!translationKeys.empty())
		{
			std::sort(translationKeys.begin(), translationKeys.end(), AnimationKeySort<TranslationKey>);
		}
		return translationKeys;
	}

	static const uint32_t k_MeshImportFlags =
		aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
		aiProcess_Triangulate |             // Make sure we're triangles
		aiProcess_FlipUVs |
		aiProcess_SortByPType |             // Split meshes by primitive type
		aiProcess_GenNormals |              // Make sure we have legit normals
		aiProcess_GenUVCoords |             // Convert UVs if required 
		//aiProcess_OptimizeGraph |
		aiProcess_OptimizeMeshes |          // Batch draws where possible
		aiProcess_JoinIdenticalVertices |
		aiProcess_LimitBoneWeights |        // If more than N (=4) bone weights, discard least influencing bones and renormalise sum to 1
		aiProcess_ValidateDataStructure |   // Validation
		aiProcess_GlobalScale;              // e.g. convert cm to m for fbx import (and other formats where cm is native)

	MeshImporter::MeshImporter(const ImportParams& params) :
		Importer(params)
	{
	}

	Ref<Asset> MeshImporter::Load()
	{
		if(m_ImportParams.Setting.Mesh.bUseMesh)
		{
			return LoadFromSubMesh();
		}
		else
		{
			return LoadFromDisk();
		}
	}

	Ref<Asset> MeshImporter::LoadFromSubMesh()
	{
		auto mesh = CreateRef<StaticMesh>(m_ImportParams);
		mesh->m_SubMeshes.emplace_back(std::move(m_ImportParams.Setting.Mesh.InitMesh));
		auto& subMesh = mesh->m_SubMeshes.back();
		subMesh.MaterialIdx = 0;

		mesh->m_Root.Name = m_ImportParams.Name;
		mesh->m_Root.SubMeshes.emplace_back(0);

		switch(m_ImportParams.Setting.Mat.Type)
		{
			case MaterialFormat_Standard:
			{
				auto mat = AssetMgr::Get()->GetAsset(Path::MaterialKey(MatAsset::STANDARD))->SpawnChild();
				mesh->m_Materials.push_back(RefCast<Asset, Material>(mat));
				break;
			}
			case MaterialFormat_Textured:
			{
				auto mat = AssetMgr::Get()->GetAsset(Path::MaterialKey(MatAsset::TEXTURED))->SpawnChild();
				mesh->m_Materials.push_back(RefCast<Asset, Material>(mat));
				break;
			}
			default:
				LOGE("err! invalid material format[%u]", m_ImportParams.Setting.Mat.Type);
				std::abort();
				break;
		}
		return mesh;
	}

	Ref<Asset> MeshImporter::LoadFromDisk()
	{
		uint32_t major = aiGetVersionMajor();
		uint32_t minor = aiGetVersionMinor();
		LOGD("begin to load mesh[%s] by assimp[%u, %u]", m_FullPath.c_str(), major, minor);
		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE |
									aiPrimitiveType_POINT);
		const auto* scene = importer.ReadFile(m_FullPath.c_str(), k_MeshImportFlags);
		if(!scene)
		{
			LOGE("fail to open mesh file[%s]", m_FullPath.c_str());
			return Ref<Asset>();
		}
		std::shared_ptr<Asset> mesh;
		if(HasAnyBone(scene))
		{
			mesh = ParseSkinnedMesh(scene);
		}
		else
		{
			mesh = ParseStaticMesh(scene);
		}
		return mesh;
	}

	bool MeshImporter::HasAnyBone(const aiScene* scene) const
	{
		if(!scene->HasMeshes())
			return false;

		for(uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			if(scene->mMeshes[i] && scene->mMeshes[i]->HasBones())
				return true;
		}
		return false;
	}

	Ref<Asset> MeshImporter::ParseStaticMesh(const aiScene* scene)
	{
		LOGD("enter");
		auto mesh = CreateRef<StaticMesh>(m_ImportParams);
		ParseMesh(scene, mesh);
		ParseRelation(scene, mesh);
		ParseMaterial(scene, mesh);
		return mesh;
	}

	Ref<Asset> MeshImporter::ParseSkinnedMesh(const aiScene* scene)
	{
		LOGD("enter");		
		auto mesh = CreateRef<SkinnedMesh>(m_ImportParams);
		ParseMesh(scene, mesh);
		ParseRelation(scene, mesh);
		ParseMaterial(scene, mesh);
		ParseSkeleton(scene, mesh);
		ParseAnimationClip(scene, mesh);
		ParseBoneInfluence(scene, mesh);
		return mesh;
	}

	void MeshImporter::ParseMesh(const aiScene* scene, const Ref<StaticMesh>& mesh)
	{
		if(!scene->HasMeshes())
		{
			LOGE("scene[%s] doesn't contain any mesh", m_FullPath);
			return;
		}
		uint32_t verticeNum = 0;
		uint32_t idxNum = 0;
		mesh->m_SubMeshes.reserve(scene->mNumMeshes);
		for(uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			auto* aiMesh = scene->mMeshes[i];

			mesh->m_SubMeshes.emplace_back();
			auto& subMesh = mesh->m_SubMeshes.back();
			subMesh.VerticeOffset = verticeNum;
			subMesh.IndexOffset = idxNum;
			subMesh.VerticeNum = aiMesh->mNumVertices;
			subMesh.IndexNum = aiMesh->mNumFaces * 3;
			subMesh.Name = aiMesh->mName.C_Str();
			subMesh.FullPath = m_ImportParams.Path;
			subMesh.FullPath.append(Path::FILE_SEPARATOR)
				.append(subMesh.Name);
			subMesh.MaterialIdx = aiMesh->mMaterialIndex;

			verticeNum += aiMesh->mNumVertices;
			idxNum += aiMesh->mNumFaces * 3;

			subMesh.Vertice.reserve(aiMesh->mNumVertices);			
			for(uint32_t i = 0; i < aiMesh->mNumVertices; i++)
			{
				Vertex v;
				v.Pos = glm::vec3(aiMesh->mVertices[i].x,
								  aiMesh->mVertices[i].y,
								  aiMesh->mVertices[i].z);
				v.Normal = glm::vec3(aiMesh->mNormals[i].x,
									 aiMesh->mNormals[i].y,
									 aiMesh->mNormals[i].z);
				if(aiMesh->HasTangentsAndBitangents())
				{
					v.Tangent = glm::vec3(aiMesh->mTangents[i].x,
										  aiMesh->mTangents[i].y,
										  aiMesh->mTangents[i].z);
				}

				if(aiMesh->HasTextureCoords(0))
				{
					v.TexCoord = glm::vec2(aiMesh->mTextureCoords[0][i].x,
										   aiMesh->mTextureCoords[0][i].y);
				}
				subMesh.Vertice.emplace_back(v);
				subMesh.BoundingBox.Update(v.Pos);
			}			

			for(uint32_t i = 0; i < aiMesh->mNumFaces; i++)
			{
				subMesh.Indice.emplace_back(aiMesh->mFaces[i].mIndices[0]);
				subMesh.Indice.emplace_back(aiMesh->mFaces[i].mIndices[1]);
				subMesh.Indice.emplace_back(aiMesh->mFaces[i].mIndices[2]);
			}

			LOGD("finish parse mesh[%u], path[%s]", i, subMesh.FullPath.c_str());
		}
	}

	void MeshImporter::ParseMaterial(const aiScene* scene, const Ref<StaticMesh>& mesh)
	{
		if(!mesh || mesh->m_SubMeshes.empty())
		{
			LOGE("mesh[%s] has not been parsed", m_FullPath.c_str());
			std::abort();
		}

		if(!scene->HasMaterials())
			return;

		for(uint32_t i = 0; i < scene->mNumMaterials; i++)
		{
			auto* aiMaterial = scene->mMaterials[i];
			aiShadingMode aiMode;
			if(aiMaterial->Get(AI_MATKEY_SHADING_MODEL, aiMode) != AI_SUCCESS)
			{
				LOGW("fail to get shading model, use pbr as default");
				aiMode = aiShadingMode_PBR_BRDF;
			}

			ImportParams param;
			param.SetContext(m_ImportParams.Context.lock())
				.SetPath(m_ImportParams.Path)
				.SetName(Path::PathWithoutSuffix(aiMaterial->GetName().C_Str()))
				.SetSuffix(Path::MAT_SUFFIX)
				.SetFmt(AssetFormat_Material)
				.SetMatPtr(aiMaterial);
			switch(aiMode)
			{
				case aiShadingMode_PBR_BRDF:
				default:
					LOGD("PBR Material,i[%u], mode[%u], path[%s], name[%s]",
						 i, aiMode, param.Path, param.Name);
					param.SetMatSpec(MaterialFormat_Standard);
					break;
			}
			auto material = AssetMgr::Get()->LoadAsset<Material>(param);
			if(mesh->IsSkinned())
			{
				material->AddFlag(ShaderPermutationFlag::USE_SKELETON);
			}
			mesh->m_Materials.push_back(material);
		}
	}

	void MeshImporter::ParseRelation(const aiScene* scene, const Ref<StaticMesh>& mesh)
	{
		if(!mesh || mesh->m_SubMeshes.empty())
		{
			LOGE("mesh has not been parsed");
			std::abort();
		}
		TraverseNode(scene->mRootNode, mesh->m_Root, mesh->m_SubMeshes, glm::mat4(1.f), 0);
	}

	void MeshImporter::ParseSkeleton(const aiScene* scene, const Ref<SkinnedMesh>& mesh)
	{
		// get unique bone names for hierarchy building
		std::map<std::string, glm::mat4> bindInfo;
		for(uint32_t i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			for(uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
			{				
				const aiBone* bone = mesh->mBones[boneIndex];
				bindInfo[bone->mName.C_Str()] = AssimpUtils::Mat4FromAiMatrix4x4(bone->mOffsetMatrix);
			}
		}

		if(bindInfo.empty())
			return;

		ImportParams param;	
		param.SetFmt(AssetFormat_Skeleton)
			.SetPath(m_ImportParams.Path)
			.SetName(m_ImportParams.Name)
			.SetSuffix(Path::SKELETON_SUFFIX)
			.SetContext(m_ImportParams.Context.lock());
		mesh->m_Skeleton = CreateRef<Skeleton>(param);

		TraverseJoint(scene->mRootNode, bindInfo, mesh->m_Skeleton);

		mesh->m_Skeleton->Init();
		AssetMgr::Get()->SaveAsset(mesh->m_Skeleton);
		//LOGD("total joint size[%u]", mesh->m_Skeleton->m_Joints.size());
		//for(size_t i = 0; i < mesh->m_Skeleton->m_Joints.size(); i++)
		//{
		//	auto& joint = mesh->m_Skeleton->m_Joints[i];
		//	LOGD("joint[%s], index[%u], parent index[%u], chilren size[%u], level[%u]", joint.Name.c_str(), i, joint.Parent, joint.Children.size(), joint.Level);
		//}
	}

	void MeshImporter::ParseAnimationClip(const aiScene* scene, const Ref<SkinnedMesh>& mesh)
	{	
		if(scene->mNumAnimations < 1)
		{
			LOGE("no animation for %s", scene->mName.C_Str());
			return;
		}

		for(int32_t i = 0; i < scene->mNumAnimations; i++)
		{
			auto* animation = scene->mAnimations[i];
			ImportParams param;
			param.SetContext(m_ImportParams.Context.lock())
				.SetPath(m_ImportParams.Path)
				.SetName(Path::PathWithoutSuffix(animation->mName.C_Str()))
				.SetSuffix(Path::ANIM_SUFFIX)
				.SetFmt(AssetFormat_AnimClip);
			auto clip = CreateRef<AnimationClip>(param);
			clip->m_FramesPerSecond = animation->mTicksPerSecond;
			clip->m_FrameCount = animation->mDuration;
			
			// mNumChannels means bone animation channel
			for(int32_t j = 0; j < animation->mNumChannels; j++)
			{
				auto* joint = animation->mChannels[j];
				if(!mesh->m_Skeleton->HasJoint(joint->mNodeName.C_Str()))
				{
					LOGW("skeleton[%s] doesn't have joint[%s]", mesh->m_Skeleton->Name().c_str(), joint->mNodeName.C_Str());
					continue;
				}
				JointSample sample;
				sample.Name = joint->mNodeName.C_Str();
				sample.Position = SplitTranslations(joint);
				sample.Rotation = SplitRotations(joint);
				sample.Scale = SplitScales(joint);
				clip->m_Samples[sample.Name] = sample;
			}
			clip->Init();
			AssetMgr::Get()->SaveAsset(clip);
			mesh->m_AnimationClips.push_back(clip);
		}
	}

	void MeshImporter::ParseBoneInfluence(const aiScene* scene, const Ref<SkinnedMesh>& mesh)
	{
		if(!mesh || !mesh->m_Skeleton || mesh->m_SubMeshes.empty())
		{
			LOGE("invalid SkinnedMesh");
			return;
		}

		mesh->m_BoneInfluences.resize(scene->mNumMeshes);
		for(uint32_t i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* aMesh = scene->mMeshes[i];
			SubMesh& subMesh = mesh->m_SubMeshes[i];
			std::vector<BoneInfluence>& boneInfluenceList = mesh->m_BoneInfluences[i];
			boneInfluenceList.resize(subMesh.VerticeNum);

			for(uint32_t boneIdx = 0; boneIdx < aMesh->mNumBones; ++boneIdx)
			{
				std::string jointName = aMesh->mBones[boneIdx]->mName.C_Str();
				int32_t jointIdx = mesh->m_Skeleton->GetJointIndex(jointName);
				if(jointIdx < 0)
				{
					LOGW("skeleton[%s] doesn't have this bone[%s], ignore", mesh->m_Skeleton->Name().c_str(), jointName.c_str());
					continue;
				}				
				auto* weights = aMesh->mBones[boneIdx]->mWeights;
				uint32_t numWeights = aMesh->mBones[boneIdx]->mNumWeights;
				for(uint32_t weightIndex = 0; weightIndex < numWeights; ++weightIndex)
				{
					int vertexId = weights[weightIndex].mVertexId;
					float weight = weights[weightIndex].mWeight;

					if(vertexId >= subMesh.VerticeNum)
					{
						LOGE("invalid vertex id[%d] for joint[%s], vertex array size[%u]", vertexId, jointName.c_str(), subMesh.VerticeNum);
						continue;
					}

					if(weight < 0.f || weight > 1.f)
					{
						LOGW("vertex[%u] of joint[%s]'s weight[%f] is out of range. clamp it to [0, 1]", vertexId, jointName.c_str(), weight);
						weight = Math::Clamp(weight, 0.f, 1.f);
					}

					auto& influence = boneInfluenceList[vertexId];
					for(int32_t index = 0; index < PIO_MAX_BONE_INFLUENCE; ++index)
					{
						if(influence.Indices[index] < 0)
						{
							influence.Indices[index] = jointIdx;
							influence.Weights[index] = weight;
							break;
						}
					}
				}
			}

			for(size_t k = 0; k < boneInfluenceList.size(); k++)
			{
				NormalizeBoneInfluence(boneInfluenceList[k]);
			}
		}
	}

	void MeshImporter::TraverseNode(aiNode* aNode, SubMeshNode& sNode, std::vector<SubMesh>& subMeshes, const glm::mat4& parentTransform, uint32_t level)
	{
		sNode.Name = aNode->mName.C_Str();
		sNode.Level = level;
		sNode.LocalTransform = AssimpUtils::Mat4FromAiMatrix4x4(aNode->mTransformation);

		auto transform = parentTransform * sNode.LocalTransform;
		for(uint32_t i = 0; i < aNode->mNumMeshes; i++)
		{
			uint32_t idx = aNode->mMeshes[i];
			auto& subMesh = subMeshes[idx];

			subMesh.Transform = transform;
			subMesh.LocalTransform = sNode.LocalTransform;
			sNode.SubMeshes.push_back(idx);
		}

		sNode.Children.resize(aNode->mNumChildren);
		for(uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			TraverseNode(aNode->mChildren[i], sNode.Children[i], subMeshes, transform, level + 1);
		}
	}	

	void MeshImporter::TraverseJoint(const aiNode* root, const std::map<std::string, glm::mat4>& bindInfo, const Ref<Skeleton>& skeleton)
	{
		struct JointBuilder
		{
			const aiNode* Node{nullptr};
			uint8_t ParentIndex{ROOT_JOINT_INDEX};
			uint8_t Level{ 0 };

			JointBuilder() {}
			JointBuilder(const aiNode* node, uint8_t parentIndex, uint8_t level) : Node(node), ParentIndex(parentIndex), Level(level) {}
		};
			
		std::queue<JointBuilder> queue;
		JointBuilder rootBuilder(root, ROOT_JOINT_INDEX, 0);
		queue.push(rootBuilder);		

		// BFS search
		while(!queue.empty())
		{
			auto b = queue.front();
			queue.pop();

			auto it = bindInfo.find(b.Node->mName.C_Str());
			if(it != bindInfo.end() && skeleton->m_JointInfo.find(b.Node->mName.C_Str()) == skeleton->m_JointInfo.end())
			{
				skeleton->m_Joints.emplace_back();
				auto& joint = skeleton->m_Joints.back();
				joint.InvBindPose = it->second;
				joint.Parent = b.ParentIndex;
				joint.Name = b.Node->mName.C_Str();
				joint.Level = b.Level;
				if(joint.Parent < skeleton->m_Joints.size())
				{
					skeleton->m_Joints[joint.Parent].Children.push_back(skeleton->m_Joints.size() - 1);
				}
				skeleton->m_JointInfo.insert({ b.Node->mName.C_Str(), skeleton->m_Joints.size() - 1 });
			}

			uint8_t parentIdx = skeleton->m_Joints.empty() ? ROOT_JOINT_INDEX : skeleton->m_Joints.size() - 1;
			for(size_t i = 0; i < b.Node->mNumChildren; i++)
			{
				JointBuilder builder(b.Node->mChildren[i], parentIdx, (parentIdx == ROOT_JOINT_INDEX) ? 0 : skeleton->m_Joints[parentIdx].Level + 1);
				queue.push(builder);
			}
		}
	}
}