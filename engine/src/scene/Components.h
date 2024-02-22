#ifndef __PIONNER_SCENE_COMPONENT_H__
#define __PIONNER_SCENE_COMPONENT_H__

#include "Registry.h"
#include "SceneDef.h"

#include "asset/Asset.h"
#include "core/math/Transform.h"
#include "gfx/struct/Camera.h"
#include "physics/PhysicsDef.h"

namespace pio
{
	struct RelationshipComponent
	{
		//[NOTE] Decs use memcpy to transfer data, so if we use std::string,
		//       string which is allocated on heap will be empty pointer
		std::string_view Tag{};
		uint32_t Self{ NullIndex };
		uint32_t ParentIndex{ NullIndex };
		std::vector<uint32_t> Children{};

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent &rhs) = default;
	};

	struct TransformComponent
	{
		Transform Transform{};		
		glm::mat4 getMat() { return Transform.getMat(); }
	};

	struct CameraComponent
	{
		bool Primary{ false };
		ProjectionType PrjType{ ProjectionType::Perspective };
		Camera Camera{};
	};

	// ---------------- Mesh related ----------------------
	// ----------------------------------------------------
	struct MeshSourceComponent
	{
		bool Visible{ true };
		AssetHandle SourceHandle{ NullAsset };
	};

	struct MeshComponent
	{
		bool Visible{ true };
		AssetHandle Handle{ NullAsset };
		AssetHandle SourceHandle{ NullAsset };
		uint32_t SubmeshIndex{ NullIndex };
		RenderState State;
		bool Delay{ false };// Only for deferred shading
	};

	struct StaticMeshComponent
	{
		bool Visible{ true };
		AssetHandle Handle{ NullAsset };
		AssetHandle SourceHandle{ NullAsset };
		uint32_t SubmeshIndex{ NullIndex };
		RenderState State;
		bool Delay{ false };// Only for deferred shading
	};

	struct C3dUIComponent
	{
		bool Visible{ true };
		std::string_view Name;
		AssetHandle Handle{ NullAsset };
		AssetHandle SourceHandle{ NullAsset };
		uint32_t SubmeshIndex{ NullIndex };
		RenderState State;
	};

	struct SpriteComponent
	{
		bool Visible{ true };
		std::string_view Name;
		AssetHandle QuadMesh{ NullAsset };
		AssetHandle Texture{ NullAsset };
		glm::vec2 Position{ 0 };
		uint32_t ScreenWidth{ 35 };
		uint32_t ScreenHeight{ 35 };	
		Rect2d Rect;
		glm::vec2 UVStart{ 0.0f, 0.0f };
		glm::vec2 UVEnd{ 1.0f, 1.0f };	
		RenderState State;
	};

	struct AnimationComponent
	{
		AssetHandle Handle{ NullAsset };
		int32_t Selection{ 0 };
		bool GPUSkinning{ true };
	};

	// ---------------- Light related ----------------------
	// -----------------------------------------------------

	struct DirectionalLightComponent
	{
		glm::vec3 Position{ 0.f };
		glm::vec3 Dest{ 0.f };
		glm::vec3 Radiance{ 0.0f, 0.0f, 0.0f };	
		float Intensity{ 0.0f };
		float Bias{ 0.0005f };
		int SdMode{ ShadowMode_Soft };
		bool CastShadow{ true };
	};

	struct PointLightComponent
	{
		uint32_t Index{ 0 };
		SphereCoord Position{};
		glm::vec3 Radiance{ 1.f };
		float Intensity{ 1.f };
		float MinRadius{ 1.f };
		float Radius{ 10.f };
		float SourceSize{ 0.5f }; // For PCSS
		bool CastsShadows{ true };
		float Falloff{ 1.0f };
		AssetHandle MeshHandle{ NullAsset };
	};

	struct SpotLightComponent
	{
		glm::vec3 Radiance{ 1.0f };
		float Intensity = 1.0f;
		float Range = 10.0f;
		float Angle = 60.0f;
		float AngleAttenuation = 5.0f;
		bool CastsShadows = false;
		float Falloff = 1.0f;
	};

	struct SkyLightComponent
	{
		UUID32 SceneEnvironment;
		float Intensity = 1.0f;
		float Lod = 0.0f;

		bool DynamicSky = false;
		glm::vec3 TurbidityAzimuthInclination = { 2.0, 0.0, 0.0 };
	};

	struct RigidBodyComponent
	{
		enum class Type { None = -1, Static, Dynamic };

		Type BodyType{ Type::Static };
		std::string_view Name{};
		glm::vec3 Center{ 0.f };
		float Density{ PIO_NORMAL_DENSITY };
	};

	struct BoxColliderComponent
	{
		glm::vec3 HalfSize{ 0.f };
		AssetHandle Material{ NullAsset };
		uint32_t ShapeFlags{ PIO_BIT(PhysicsShapeFlag::Visualize) | PIO_BIT(PhysicsShapeFlag::Query) | PIO_BIT(PhysicsShapeFlag::Simulate)};
	};

	struct SphereColliderComponent
	{
	};

	struct CapsuleColliderComponent
	{
	};

	struct MeshColliderComponent
	{
	};

	struct ProxyComponent
	{
		Ref<Entity> Proxy{};
	};

	struct SceneComponent
	{
		// Scene Control Related
		Ref<Entity> Selected3D{};

		// Physics Related
		AssetHandle PhycisScene;
		bool Simulate{ true };
		bool Primary{ false };
	};

	// Singleton Component
	struct EditorComponent
	{
		uint32_t SelectedEntIndex{ NullIndex };
	};

	#define PIO_RELATION_SET_TAG(pEnt, tag) if(pEnt && pEnt->hasComponent<RelationshipComponent>()) { pEnt->getComponent<RelationshipComponent>().Tag = tag; } 
	#define PIO_RELATION_SET_SELF_INDEX(pEnt, self) if(pEnt && pEnt->hasComponent<RelationshipComponent>())\
										      { pEnt->getComponent<RelationshipComponent>().Self = self; }
	#define PIO_RELATION_SET_PARENT_INDEX(pEnt, parentIndex) if(pEnt && pEnt->hasComponent<RelationshipComponent>())\
													   { pEnt->getComponent<RelationshipComponent>().ParentIndex = parentIndex; }
	#define PIO_RELATION_SET_CHILD_INDEX(pEnt, childIndex)  if(pEnt && pEnt->hasComponent<RelationshipComponent>())\
													  {\
														auto &comp = pEnt->getComponent<RelationshipComponent>();\
														auto &children = comp.Children;\
														auto it = std::find(children.begin(), children.end(), childIndex);\
														if(it == children.end()) { children.push_back(childIndex); }\
												      }
}

#endif