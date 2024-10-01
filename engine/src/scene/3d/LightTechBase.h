#ifndef __PIONNER_SCENE_3D_LIGHT_TECH_BASE_H__
#define __PIONNER_SCENE_3D_LIGHT_TECH_BASE_H__

#include "gfx/GfxDef.h"
#include "asset/Asset.h"

namespace pio
{
	#define PIO_LIGH_TECH_IMPL_TYPE(TypeName)  private:\
											   static  LightTech StaticLightTech() { return TypeName; }\
											   public:\
											   virtual LightTech lightTech() const override { return StaticLightTech(); }

	class CameraNode;
	class RenderContext;

	class LightTechBase : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::LightTech)
	public:
		LightTechBase();
		virtual ~LightTechBase();
		
		virtual LightTech lightTech() const { return LightTech::Num; }

	public:
		static Ref<LightTechBase> Create(LightTech type);
	};

	template<>
	bool Asset::is<LightTechBase>() const;
}

#endif