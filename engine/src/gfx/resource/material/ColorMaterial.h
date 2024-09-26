#ifndef __PIONNER_GFX_RESOURCES_MATERIAL_COLOR_MATERIAL_H__
#define __PIONNER_GFX_RESOURCES_MATERIAL_COLOR_MATERIAL_H__

#include "Material.h"

namespace pio
{
	class ColorMaterial : public Material
	{
	public:
		ColorMaterial(const std::string& name, RenderingMode mode = RenderingMode_Overlay);
		ColorMaterial(const ColorMaterial& rhs);
		ColorMaterial& operator=(const ColorMaterial& rhs);
		~ColorMaterial() = default;

		virtual void update(Ref<RenderContext>& context) override;
		virtual Ref<Asset> clone() const override;

		void setColor(const glm::vec3& color) { m_color = color; }		

	private:
		struct GpuAttr
		{
			static constexpr char* Color = "u_colorMaterial.Color";
		};

	private:
		glm::vec3 m_color{ 1.f };
	};

	template<>
	bool Asset::is<ColorMaterial>() const;
}

#endif