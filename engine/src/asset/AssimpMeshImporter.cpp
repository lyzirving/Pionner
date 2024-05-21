#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/version.h>
#include <limits>

#include "AssimpMeshImporter.h"
#include "AssimpAnimationImporter.h"
#include "AssimpUtils.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/struct/Mesh.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/Material.h"

#include "asset/AssetsManager.h"
#include "asset/ImageUtils.h"

#include "animation/Animation.h"
#include "animation/Skeleton.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AssimpMeshImporter"

namespace pio
{
	static const uint32_t s_meshImportFlags =
		aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
		aiProcess_Triangulate  |            // Make sure we're triangles
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

	AssimpMeshImporter::AssimpMeshImporter(const std::string &name, AssetFmt fmt) 
		: m_name(name)
		, m_assetsDir(AssetsManager::AssetsRootDirectory(fmt) + "/" + name + "/")
		, m_fmt(fmt)
	{
	}

	AssimpMeshImporter::~AssimpMeshImporter()
	{
	}

	Ref<Asset> AssimpMeshImporter::importToMeshSource()
	{
		std::string absPath;
		if (!AssetsManager::FindAssetAbsPath(m_assetsDir, m_fmt, absPath))
		{
			LOGE("fail to find assets[%s][%s]", m_assetsDir.c_str(), AssetsManager::GetFmtPostfix(m_fmt));
			return Ref<Asset>();
		}

		uint32_t major = aiGetVersionMajor(); uint32_t minor = aiGetVersionMinor();
		LOGD("begin to load [%s] by assimp[%d.%d]", absPath.c_str(), major, minor);

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(absPath.c_str(), s_meshImportFlags);
		// NOTE: scene can legit contain no meshes (e.g. it could contain an armature, an animation, and no skin (mesh)))
		if (!scene)  
		{
			LOGE("fail to load mesh file[%s]", absPath.c_str());
			return nullptr;
		}

		auto meshSource = CreateRef<MeshSource>(m_name);
		parseMeshes(scene, meshSource);
		parseSkeleton(scene, meshSource);
		parseAnimation(scene, meshSource);
		parseBoneInfluence(scene, meshSource);
		parseMaterial(scene, meshSource);
		createBuffers(scene, meshSource);
		createGPUSkinning(meshSource);
		AssetsManager::Get()->addRuntimeAsset(meshSource);
		return meshSource;
	}

	void AssimpMeshImporter::parseSkeleton(const aiScene *scene, Ref<MeshSource> &meshSource)
	{
		if (!scene || !meshSource)
			return;

		meshSource->m_skeleton = AssimpAnimationImporter::ImportSkeleton(scene);
	}

	void AssimpMeshImporter::parseAnimation(const aiScene *scene, Ref<MeshSource> &meshSource)
	{
		if (!scene || !meshSource || !meshSource->hasSkeleton())
			return;

		meshSource->m_animations = AssimpAnimationImporter::ImportAnimation(scene, meshSource->m_skeleton);
	}

	void AssimpMeshImporter::parseMeshes(const aiScene *scene, Ref<MeshSource> &meshSource)
	{
		if (scene->HasMeshes())
		{
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;

			meshSource->m_boundingBox.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
			meshSource->m_boundingBox.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

			// ---------------- Parsing meshes start ------------------------
			meshSource->m_submeshes.reserve(scene->mNumMeshes);
			for (unsigned m = 0; m < scene->mNumMeshes; m++)
			{
				// Parse a single mesh
				aiMesh *mesh = scene->mMeshes[m];

				meshSource->m_submeshes.emplace_back();
				Submesh &submesh = meshSource->m_submeshes.back();
				submesh.VertexOffset = vertexCount;
				submesh.IndexOffset = indexCount;
				submesh.MaterialIndex = mesh->mMaterialIndex;
				submesh.VertexCount = mesh->mNumVertices;
				submesh.IndexCount = mesh->mNumFaces;
				submesh.MeshName = mesh->mName.C_Str();

				vertexCount += mesh->mNumVertices;
				indexCount  += submesh.IndexCount;

				auto &aabb = submesh.BoundingBox;
				aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
				aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

				// Vertices
				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					Vertex vertex{};
					vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
					vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

					aabb.Min = glm::min(aabb.Min, vertex.Position);
					aabb.Max = glm::max(aabb.Max, vertex.Position);

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
						vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
					}

					if (mesh->HasTextureCoords(0))
						vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

					// Import Vertices
					meshSource->m_vertices.push_back(vertex);
				}

				// Indices
				for (size_t i = 0; i < mesh->mNumFaces; i++)
				{
					Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
					meshSource->m_indices.push_back(index);

					// m is the index of mesh, which is the same as submesh
					meshSource->m_triangleCache[m].emplace_back(meshSource->m_vertices[index.V0 + submesh.VertexOffset], 
																meshSource->m_vertices[index.V1 + submesh.VertexOffset],
																meshSource->m_vertices[index.V2 + submesh.VertexOffset]);
				}
			}
			// ---------------------- Parsing meshes end ------------------------------

			// ------------------ Building nodes hierarchy start ----------------------
			meshSource->m_nodes.emplace_back();
			MeshNode &rootNode = meshSource->m_nodes.back();
			traverseNodes(meshSource, scene->mRootNode, 0, glm::mat4(1.f), 0);
			// ------------------ Building nodes hierarchy end ------------------------

			// ---------- Updating the total bounding box of the meshes ---------------
			for (const auto &submesh : meshSource->m_submeshes)
			{
				AABB transformedSubmeshAABB = submesh.BoundingBox;
				glm::vec3 min = glm::vec3(submesh.Transform * glm::vec4(transformedSubmeshAABB.Min, 1.0f));
				glm::vec3 max = glm::vec3(submesh.Transform * glm::vec4(transformedSubmeshAABB.Max, 1.0f));

				meshSource->m_boundingBox.Min = glm::min(meshSource->m_boundingBox.Min, min);
				meshSource->m_boundingBox.Max = glm::max(meshSource->m_boundingBox.Max, max);
			}
			// ------------------------------------------------------------------------
		}
	}

	void AssimpMeshImporter::parseBoneInfluence(const aiScene *scene, Ref<MeshSource> &meshSource)
	{
		if (!scene || !meshSource)
			return;

		if (!meshSource->hasSkeleton() || meshSource->getSubmeshes().empty())
			return;
		
		Ref<Skeleton> skeleton = meshSource->m_skeleton;

		for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh *mesh = scene->mMeshes[i];
			Submesh &submesh = const_cast<Submesh &>(meshSource->getSubmeshes()[i]);
			submesh.IsRigged = true;
			submesh.BoneInfluences.resize(submesh.VertexCount);

			for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
			{
				std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
				auto *weights = mesh->mBones[boneIndex]->mWeights;
				int numWeights = mesh->mBones[boneIndex]->mNumWeights;

				uint32_t boneId = skeleton->getBoneInfoId(boneName);
				if (boneId == NullIndex)
				{
					boneId = skeleton->updateBoneInformation(boneName, AssimpUtils::Mat4FromAiMatrix4x4(mesh->mBones[boneIndex]->mOffsetMatrix));
					LOGW("insert bone information for bone[%s], id[%u]", boneName.c_str(), boneId);
				}

				for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
				{
					int vertexId = weights[weightIndex].mVertexId;
					float weight = weights[weightIndex].mWeight;

					if (vertexId >= submesh.VertexCount)
					{
						LOGE("invalid vertex id[%d] for bone[%s], vertex array size[%u]", vertexId, boneName.c_str(), submesh.VertexCount);
						continue;
					}

					if (weight < 0.f || weight > 1.f)
					{
						LOGW("bone[%s] for vertex[%u]'s weight[%f] is out of range. clamp it to [0, 1]", boneName.c_str(), vertexId, weight);
						weight = Math::Clamp(weight, 0.f, 1.f);
					}

					if (Math::IsZero(weight))
						continue;

					BoneInfluence &influence = submesh.BoneInfluences[vertexId];
					for (int i = 0; i < PIO_MAX_BONE_INFLUENCE; ++i)
					{
						if (influence.BoneInfoIndices[i] == NullIndex)
						{
							influence.BoneInfoIndices[i] = boneId;
							influence.Weights[i] = weight;
							break;
						}
					}
				}
			}

			for (uint32_t i = 0; i < submesh.BoneInfluences.size(); i++)
			{
				submesh.BoneInfluences[i].normalizeWeights();
			}
		}
	}

	void AssimpMeshImporter::parseMaterial(const aiScene *scene, Ref<MeshSource> &meshSource)
	{
		if (!scene || !meshSource)
			return;

		glm::vec3 albedoColor{ 0.8f }, emission{ 0.f };
		float roughness{ 0.5f }, metalness{ 0.f };

		Ref<Texture2D> whiteTexture = Renderer::GetWhiteTexture();

		if (scene->HasMaterials())
		{
			meshSource->m_materials.reserve(scene->mNumMaterials);
			for (uint32_t i = 0; i < scene->mNumMaterials; i++)
			{
				auto aiMaterial = scene->mMaterials[i];
				auto aiMaterialName = aiMaterial->GetName();

				if (aiMaterialName.length == 0)
					continue;

				auto mi = Material::Create(aiMaterialName.C_Str());
				meshSource->m_materials.push_back(mi);

				aiColor3D aiColor, aiEmission;
				ai_real aiFloat{ 0.f };
				ai_int  aiInt{ 0 };
				if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS)
					albedoColor = { aiColor.r, aiColor.g, aiColor.b };

				if (aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmission) == AI_SUCCESS)
					emission = glm::vec3(aiEmission.r, aiEmission.g, aiEmission.b);

				if (aiMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, aiFloat) == AI_SUCCESS)
					roughness = aiFloat;

				if (aiMaterial->Get(AI_MATKEY_METALLIC_FACTOR, aiFloat) == AI_SUCCESS)
					metalness = aiFloat;

				aiString aiTexPath;
				bool hasAlbedoMap = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
				bool fallback = !hasAlbedoMap;
				if (hasAlbedoMap)
				{
					TextureSpecification spec;
					spec.Name = aiTexPath.C_Str();				
					spec.Format = ImageInternalFormat::FROM_FILE;

					std::string path = m_assetsDir + spec.Name;
					Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);
					if (texture)
					{
						mi->set(MaterialAttrs::MU_AlbedoTexture, texture);
						mi->set(MaterialAttrs::MU_AlbedoColor, albedoColor);
					}
					else
					{
						LOGE("mesh failed to load albedo texture[%s]", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					mi->set(MaterialAttrs::MU_AlbedoTexture, whiteTexture);
					mi->set(MaterialAttrs::MU_AlbedoColor, albedoColor);
				}

				bool hasEmissionMap = (aiMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &aiTexPath) == AI_SUCCESS) ||
									  (aiMaterial->GetTexture(aiTextureType_EMISSION_COLOR, 0, &aiTexPath) == AI_SUCCESS);
				fallback = !hasEmissionMap;
				if (hasEmissionMap)
				{
					TextureSpecification spec;
					spec.Name = aiTexPath.C_Str();			
					spec.Format = ImageInternalFormat::FROM_FILE;

					std::string path = m_assetsDir + spec.Name;
					Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);
					if (texture)
					{
						mi->set(MaterialAttrs::MU_EmissionTexture, texture);
						mi->set(MaterialAttrs::MU_Emission, emission);
					}
					else
					{
						LOGE("mesh failed to load emission texture[%s]", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					mi->set(MaterialAttrs::MU_EmissionTexture, whiteTexture);
					mi->set(MaterialAttrs::MU_Emission, emission);
				}

				bool hasNormalMap = aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS;
				fallback = !hasNormalMap;
				if (hasNormalMap)
				{
					TextureSpecification spec;
					spec.Name = aiTexPath.C_Str();			
					spec.Format = ImageInternalFormat::FROM_FILE;

					std::string path = m_assetsDir + spec.Name;
					Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);
					if (texture)
					{
						mi->set(MaterialAttrs::MU_NormalTexture, texture);
						mi->set(MaterialAttrs::MU_UseNormalMap, true);
					}
					else
					{
						LOGE("mesh failed to load normal map[%s]", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					mi->set(MaterialAttrs::MU_NormalTexture, whiteTexture);
					mi->set(MaterialAttrs::MU_UseNormalMap, false);
				}			
				
				std::string metalRoughnessTexName{ aiMaterialName.C_Str() }; metalRoughnessTexName += "_metallicRoughness";
				Ref<Texture2D> metallicRoughnessTex{};
				bool hasMetallicMap = aiMaterial->GetTexture(AI_MATKEY_METALLIC_TEXTURE, &aiTexPath) == AI_SUCCESS;
				bool hasMetallicRoughness = hasMetallicMap && StringUtil::contains(aiTexPath.C_Str(), "roughness");				
				fallback = !hasMetallicMap && !hasMetallicRoughness;				
				if (hasMetallicRoughness)
				{			
					TextureSpecification spec;
					spec.Name = aiTexPath.C_Str();				
					spec.Format = ImageInternalFormat::FROM_FILE;

					std::string path = m_assetsDir + spec.Name;
					Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);
					if (texture)
					{

						mi->set(MaterialAttrs::MU_MetallicRoughnessTexture, texture);
						mi->set(MaterialAttrs::MU_Metalness, metalness);
						mi->set(MaterialAttrs::MU_Roughness, roughness);
					}
					else
					{
						LOGE("mesh failed to load metallic-roughness texture from file [%s]", aiTexPath.C_Str());
						fallback = true;
					}
				}
				else if (hasMetallicMap)
				{									
					std::string path = m_assetsDir + aiTexPath.C_Str();
					int32_t width{ 0 }, height{ 0 }, component{ 0 };

					if (ImageUtils::GetPicInfo(path.c_str(), width, height, component))
					{
						metallicRoughnessTex = Texture2D::Create(width, height, 3, 255, metalRoughnessTexName);						
						AssetsManager::Get()->addPackedAsset(metalRoughnessTexName, metallicRoughnessTex);
					}
					else
					{
						LOGE("fail to get image info for metallic-roughness[%f] when paring metallic tex", path.c_str());
						fallback = true;
					}

					if (metallicRoughnessTex)
					{
						// Read R channel for metallic
						uint8_t *data = stbi_load(path.c_str(), &width, &height, &component, 1);
						fallback = !(data && ImageUtils::FillChannelData(data, metallicRoughnessTex->getBuffer()->as<uint8_t>(), width, height, 3, 1));
						if (!fallback)
						{
							mi->set(MaterialAttrs::MU_MetallicRoughnessTexture, metallicRoughnessTex);
							mi->set(MaterialAttrs::MU_Metalness, metalness);
						}
						else
						{
							LOGE("fail to get image's R channel data for metallic [%s]", path.c_str());
						}	

						if (data) { stbi_image_free(data); }
					}
				}

				bool hasRoughnessMap = aiMaterial->GetTexture(AI_MATKEY_ROUGHNESS_TEXTURE, &aiTexPath) == AI_SUCCESS;
				// Read roughness if need
				if ((fallback || (hasMetallicMap && !hasMetallicRoughness)) && hasRoughnessMap)
				{
					std::string path = m_assetsDir + aiTexPath.C_Str();
					int32_t width{ 0 }, height{ 0 }, component{ 0 };
					if (!metallicRoughnessTex)
					{
						if (ImageUtils::GetPicInfo(path.c_str(), width, height, component))
						{
							metallicRoughnessTex = Texture2D::Create(width, height, 3, 255, metalRoughnessTexName);
							AssetsManager::Get()->addPackedAsset(metalRoughnessTexName, metallicRoughnessTex);
						}
						else
						{
							LOGE("fail to get image info for metallic-roughness[%f] when paring roughness tex", path.c_str());
							fallback = true;
						}
					}

					if (metallicRoughnessTex)
					{
						// Read G channel for roughness
						uint8_t *data = stbi_load(path.c_str(), &width, &height, &component, 2);
						fallback = !(data && ImageUtils::FillChannelData(data, metallicRoughnessTex->getBuffer()->as<uint8_t>(), width, height, 3, 2));
						if (!fallback)
						{
							mi->set(MaterialAttrs::MU_MetallicRoughnessTexture, metallicRoughnessTex);
							mi->set(MaterialAttrs::MU_Roughness, roughness);
						}
						else
						{
							LOGE("fail to get image's G channel data for roughness [%s]", path.c_str());
						}
						if (data) { stbi_image_free(data); }
					}
				}
								
				if (fallback)
				{
					if (!mi->hasTexture2D(MaterialAttrs::MU_MetallicRoughnessTexture))
						mi->set(MaterialAttrs::MU_MetallicRoughnessTexture, whiteTexture);
					mi->set(MaterialAttrs::MU_Metalness, metalness);
					mi->set(MaterialAttrs::MU_Roughness, roughness);
				}

				// TODO: parse ambient occlusion
				mi->set(MaterialAttrs::MU_AOTexture, whiteTexture);
				mi->set(MaterialAttrs::MU_AO, 1.f);
			}
			meshSource->m_materials.shrink_to_fit();
		}
		else
		{
			// mesh has no material, create default one 
			auto mi = Material::Create("default material");
			mi->set(MaterialAttrs::MU_AlbedoColor, albedoColor);
			mi->set(MaterialAttrs::MU_Emission, emission);
			mi->set(MaterialAttrs::MU_Metalness, metalness);
			mi->set(MaterialAttrs::MU_Roughness, roughness);
			mi->set(MaterialAttrs::MU_UseNormalMap, false);
			mi->set(MaterialAttrs::MU_AlbedoTexture, whiteTexture);			
			mi->set(MaterialAttrs::MU_NormalTexture, whiteTexture);
			mi->set(MaterialAttrs::MU_MetallicRoughnessTexture, whiteTexture);
			meshSource->m_materials.push_back(mi);
		}
	}

	void AssimpMeshImporter::createBuffers(const aiScene *scene, Ref<MeshSource> &meshSource)
	{
		for (uint32_t i = 0; i < meshSource->m_submeshes.size(); i++)
		{
			auto &mesh = meshSource->m_submeshes[i];
			if (mesh.VertexCount > 0)
			{
				mesh.VertexArray = VertexArray::Create();

				mesh.VertexBuffer = VertexBuffer::Create(&(meshSource->m_vertices[mesh.VertexOffset]),
														 mesh.VertexCount * sizeof(Vertex));
				mesh.VertexBuffer->setLayout(VertexBuffer::To<Vertex>());

				mesh.IndexBuffer = IndexBuffer::Create(&(meshSource->m_indices[mesh.IndexOffset]),
													   mesh.IndexCount * sizeof(Index),
													   mesh.IndexCount * 3);

				mesh.VertexArray->addVertexBuffer(mesh.VertexBuffer);

				if (!mesh.BoneInfluences.empty())
				{
					mesh.BoneInfluenceBuffer = VertexBuffer::Create(&(mesh.BoneInfluences[0]), mesh.VertexCount * sizeof(BoneInfluence));
					mesh.BoneInfluenceBuffer->setLayout(VertexBuffer::To<BoneInfluence>());
					mesh.VertexArray->addVertexBuffer(mesh.BoneInfluenceBuffer);
				}
			}
		}
	}

	void AssimpMeshImporter::createGPUSkinning(Ref<MeshSource> &meshSource)
	{
		PIO_ASSERT_RETURN(meshSource.use_count() != 0, "mesh source is invalid");
		PIO_ASSERT_RETURN(!meshSource->m_nodes.empty(), "invalid node hierarchy");
		for (uint32_t i = 0; i < meshSource->m_animations.size(); i++)
		{
			Ref<Animation> &anim = meshSource->m_animations[i];
			if (!anim)
				continue;
			anim->buildGPUSkinning(meshSource->m_skeleton, meshSource->m_nodes);
		}
	}

	void AssimpMeshImporter::traverseNodes(Ref<MeshSource> &meshSource, void *assimpNode, uint32_t nodeIndex, 
										   const glm::mat4 &parentTransform, uint32_t level)
	{
		aiNode *aNode = (aiNode *)assimpNode;

		MeshNode &node = meshSource->m_nodes[nodeIndex];
		node.Name = aNode->mName.C_Str();
		node.LocalTransform = AssimpUtils::Mat4FromAiMatrix4x4(aNode->mTransformation);
		node.Level = level;

		// World transform: passed from root
		glm::mat4 transform = parentTransform * node.LocalTransform;
		// Meshes releated with Node
		for (uint32_t i = 0; i < aNode->mNumMeshes; i++)
		{
			uint32_t submeshIndex = aNode->mMeshes[i];
			auto &submesh = meshSource->m_submeshes[submeshIndex];
			submesh.NodeName = aNode->mName.C_Str();
			submesh.Transform = transform;
			submesh.LocalTransform = node.LocalTransform;

			node.Submeshes.push_back(submeshIndex);
		}

		node.Children.resize(aNode->mNumChildren);
		uint32_t parentNodeIndex = (uint32_t)meshSource->m_nodes.size() - 1;

		// Parsing the node's children iteratively
		for (uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			meshSource->m_nodes.emplace_back();
			MeshNode &child = meshSource->m_nodes.back();
			uint32_t childIndex = meshSource->m_nodes.size() - 1;

			child.Parent = parentNodeIndex;
			meshSource->m_nodes[nodeIndex].Children[i] = childIndex;
			traverseNodes(meshSource, aNode->mChildren[i], childIndex, transform, level + 1);
		}
	}
}
