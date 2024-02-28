#ifndef __PIONNER_GFX_RHI_MATERIAL_H__
#define __PIONNER_GFX_RHI_MATERIAL_H__

#include "RhiDef.h"

namespace pio
{
	namespace MaterialAttrs
	{
		static const char *MU_AlbedoColor = "u_material.AlbedoColor";
		static const char *MU_Emission = "u_material.Emission";
		static const char *MU_Roughness = "u_material.Roughness";
		static const char *MU_Metalness = "u_material.Metalness";
		static const char *MU_AO = "u_material.AO";
		static const char *MU_Alpha = "u_material.Alpha";
		static const char *MU_UseNormalMap = "u_material.UseNormalMap";

		static const char *MU_AlbedoTexture = "u_material.AlbedoTexture";
		static const char *MU_EmissionTexture = "u_material.EmissionTexture";
		static const char *MU_RoughnessTexture = "u_material.RoughnessTexture";
		static const char *MU_MetalnessTexture = "u_material.MetalnessTexture";
		static const char *MU_NormalTexture = "u_material.NormalTexture";
		static const char *MU_AOTexture = "u_material.AOTexture";
	}

	class Texture;
	class Texture2D;
	class Shader;

	enum class MaterialFlag
	{
		None = PIO_BIT(0),
		DepthTest = PIO_BIT(1),
		Blend = PIO_BIT(2),
		TwoSided = PIO_BIT(3),
		ShadowCasting = PIO_BIT(4)
	};

	class Material
	{
		PIO_UNIFORM_WRAPPER_INTERFACE_DECLARE(Material)
	public:
		static Ref<Material> Create(const std::string &name);
	public:
		Material() {}
		virtual ~Material() = default;

		virtual void bind(const Ref<Shader> &shader) = 0;
		virtual void unbind() = 0;

		virtual void set(const std::string &name, const Ref<Texture2D> &texture) = 0;
		virtual Ref<Texture2D> getTexture2D(const std::string &name) = 0;

		virtual const std::string &getName() const = 0;

		virtual bool getFlag(MaterialFlag flag) const = 0;
		virtual void setFlag(MaterialFlag flag, bool value = true) = 0;

		virtual Ref<Material> copy() = 0;
	};
}

#endif