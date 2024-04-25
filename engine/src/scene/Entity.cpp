#include "Entity.h"

#include "Registry.h"
#include "Components.h"

#include "physics/PhysicsScene.h"
#include "physics/PhysicsActor.h"

#include "gfx/struct/Mesh.h"
#include "asset/AssetsManager.h"

namespace pio
{
	Registry *Entity::s_registry = Registry::Get();

	Entity::Entity(NodeType type) : m_uid(), m_handle(), m_nodeType(type)
	{
	}

	Entity::Entity(const decs::EntityID &index, NodeType type) : m_uid(), m_handle(index), m_nodeType(type)
	{
	}

	Entity::Entity(const Entity &rhs) : m_uid(rhs.m_uid), m_handle(rhs.m_handle), m_nodeType(rhs.m_nodeType)
	{
	}

	Entity::Entity(Entity &&rhs) noexcept : m_uid(std::move(rhs.m_uid)), 
		                                    m_handle(std::move(rhs.m_handle)),
		                                    m_nodeType(std::move(rhs.m_nodeType))
	{
	}

	Entity &Entity::operator=(const Entity &rhs)
	{
		if (this != &rhs)
		{
			m_uid = rhs.m_uid;
			m_handle = rhs.m_handle;
			m_nodeType = rhs.m_nodeType;
		}
		return *this;
	}

	Entity &Entity::operator=(Entity &&rhs) noexcept
	{
		if (this != &rhs)
		{
			m_uid = std::move(rhs.m_uid);
			m_handle = std::move(rhs.m_handle);
			m_nodeType = std::move(rhs.m_nodeType);
		}
		return *this;
	}

	bool Entity::operator==(const Entity &rhs)
	{
		return m_uid == rhs.m_uid &&
			   m_handle.index == rhs.m_handle.index &&
			   m_handle.generation == rhs.m_handle.generation &&
			   m_nodeType == rhs.m_nodeType;
	}

	bool Entity::operator!=(const Entity &rhs)
	{
		return !((*this) == rhs);
	}

	bool Entity::getGlobalPose(glm::vec3 &out)
	{
		switch (m_nodeType)
		{
			case pio::NodeType::MeshSource:
			{
				if (hasComponent<MeshSourceComponent>())
				{
					Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(getComponent<MeshSourceComponent>().SourceHandle);
					// Root Node's position is always world origin
					out = meshSrc->GlobalPose.mat() * glm::vec4(0.f, 0.f, 0.f, 1.f);
					return true;
				}
				break;
			}
			case pio::NodeType::Mesh:
			{
				if (hasComponent<MeshComponent>())
				{
					MeshComponent &meshComp = getComponent<MeshComponent>();
					TransformComponent &transComp = getComponent<TransformComponent>();
					auto meshSource = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);
					const Submesh &submesh = meshSource->getSubmeshes()[meshComp.SubmeshIndex];
					// The out position does not consider the effect of animation.
					out = meshSource->GlobalPose.mat() * transComp.mat() * glm::vec4(glm::vec3(0.f), 1.f);
					return true;
				}
				else if (hasComponent<StaticMeshComponent>())
				{
					StaticMeshComponent &comp = getComponent<StaticMeshComponent>();
					TransformComponent &transComp = getComponent<TransformComponent>();
					auto meshSource = AssetsManager::GetRuntimeAsset<MeshSource>(comp.SourceHandle);
					const Submesh &submesh = meshSource->getSubmeshes()[0];
					out = meshSource->GlobalPose.mat() * transComp.mat() * glm::vec4(glm::vec3(0.f), 1.f);
					return true;
				}
				break;
			}
			default:
				break;
		}
		return false;
	}

	bool Entity::setSelection(bool select)
	{
		switch (m_nodeType)
		{
			case pio::NodeType::MeshSource:
			{
				if (hasComponent<MeshSourceComponent>())
				{
					auto meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(getComponent<MeshSourceComponent>().SourceHandle);
					const std::vector<Submesh> &mesh = meshSrc->getSubmeshes();
					for (uint32_t i = 0; i < mesh.size(); i++)
					{
						const Submesh &sub = mesh[i];
						if (sub.Ent->hasComponent<MeshComponent>())
						{
							sub.Ent->getComponent<MeshComponent>().State.Selected = select;
						}
						else if (sub.Ent->hasComponent<StaticMeshComponent>())
						{
							sub.Ent->getComponent<StaticMeshComponent>().State.Selected = select;
						}
					}
					return true;
				}
				break;
			}				
			case pio::NodeType::Mesh:
			{
				if (hasComponent<MeshComponent>())
				{
					getComponent<MeshComponent>().State.Selected = select;
					return true;
				}
				else if (hasComponent<StaticMeshComponent>())
				{
					getComponent<StaticMeshComponent>().State.Selected = select;
					return true;
				}
				break;
			}
			default:
				break;
		}		
		return false;
	}

	bool Entity::setGlobalPose(const glm::vec3 &translation, const glm::vec3 &euler)
	{
		switch (m_nodeType)
		{
			case pio::NodeType::MeshSource:
			{
				if (hasComponent<MeshSourceComponent>())
				{
					auto &comp = getComponent<MeshSourceComponent>();
					Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(comp.SourceHandle);
					meshSrc->GlobalPose.Position = translation;
					meshSrc->GlobalPose.Euler = euler;
					return true;
				}
				break;
			}
			case pio::NodeType::Mesh:
			{
				if (hasComponent<TransformComponent>())
				{
					auto &comp = getComponent<TransformComponent>();
					comp.Transform.Position = translation;
					comp.Transform.Euler = euler;
					return true;
				}
				break;
			}
			case pio::NodeType::PointLight:
			{
				if (hasComponent<PointLightComponent>())
				{
					auto &comp = getComponent<PointLightComponent>();
					comp.Position = translation;
					return true;
				}
				break;
			}
			default:
				break;
		}
		return false;
	}

	bool Entity::setGlobalPoseDiff(const glm::vec3 &diff, const glm::vec3 &eulerDiff)
	{
		switch (m_nodeType)
		{
			case pio::NodeType::MeshSource:
			{
				if (hasComponent<MeshSourceComponent>())
				{
					auto &comp = getComponent<MeshSourceComponent>();
					Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(comp.SourceHandle);
					meshSrc->GlobalPose.Position += diff;
					meshSrc->GlobalPose.Euler += eulerDiff;
					return true;
				}
				break;
			}
			case pio::NodeType::Mesh:
			{
				if (hasComponent<TransformComponent>())
				{
					auto &comp = getComponent<TransformComponent>();
					comp.Transform.Position += diff;
					comp.Transform.Euler += eulerDiff;
					return true;
				}
				break;
			}
			case pio::NodeType::PointLight:
			{
				if (hasComponent<PointLightComponent>())
				{
					auto &comp = getComponent<PointLightComponent>();
					comp.Position += diff;
					return true;
				}
				break;
			}
			default:
				break;
		}
		return false;
	}

	bool Entity::setActorGlobalPose(const glm::vec3 &translation, const glm::quat &rotation)
	{
		Ref<PhysicsActor> ac;
		if (acquireActor(ac))
		{
			ac->setGlobalPose(translation, rotation);
			return true;	
		}
		return false;
	}
}