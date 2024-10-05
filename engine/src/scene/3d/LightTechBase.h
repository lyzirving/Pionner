#ifndef __PIONNER_SCENE_3D_LIGHT_TECH_BASE_H__
#define __PIONNER_SCENE_3D_LIGHT_TECH_BASE_H__

#include "gfx/rhi/RenderStateAttrs.h"
#include "asset/Asset.h"

namespace pio
{
	#define PIO_LIGH_TECH_IMPL_TYPE(TypeName)  private:\
											   static  LightTech StaticLightTech() { return TypeName; }\
											   public:\
											   virtual LightTech lightTech() const override { return StaticLightTech(); }

	class CameraNode;
	class Shader;
	class FrameBuffer;
	class UniformBuffer;
	class RenderContext;	

	class LightTechBase : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::LightTech)
	public:
		LightTechBase();
		virtual ~LightTechBase();
		
		virtual bool bind(Ref<Shader>& shader) = 0;
		virtual bool bindUnimBlock(Ref<RenderContext>& context, Ref<Shader>& shader) = 0;
		virtual void unbindUnimBlock() = 0;
		virtual Ref<UniformBuffer>& UBuff() = 0;
		virtual Ref<FrameBuffer>& frameBuff() = 0;
		virtual LightTech lightTech() const = 0;		

		const RenderStateAttrs& renderState() const { return m_attrs; }
	protected:
		RenderStateAttrs m_attrs;

	public:
		static Ref<LightTechBase> Create(Ref<RenderContext>& context, LightTech type);
	};

	template<>
	bool Asset::is<LightTechBase>() const;
}

#endif