#include "ColorMaterial.h"

#include "gfx/renderer/RenderContext.h"

namespace pio
{
	ColorMaterial::ColorMaterial(const std::string& name, RenderingMode mode)
		: Material(name, mode, ShaderSpec_Color)
	{
	}

	ColorMaterial::ColorMaterial(const ColorMaterial& rhs) : Material(rhs)
	{
		m_color = rhs.m_color;
	}

	ColorMaterial& ColorMaterial::operator=(const ColorMaterial& rhs)
	{
		if (this != &rhs)
		{
			this->ColorMaterial::ColorMaterial(rhs);
		}
		return *this;
	}

	void ColorMaterial::onUpdate(Ref<RenderContext>& context)
	{
		updateUnimData(GpuAttr::Color, m_color);
	}

	Ref<Asset> ColorMaterial::clone() const
	{
		auto material = CreateRef<ColorMaterial>(m_name, m_renderingMode);
		*(material.get()) = *this;
		material->m_hnd = UUID32();
		return material;
	}

	template<>
	bool Asset::is<ColorMaterial>() const
	{
		if (is<Material>())
		{
			return as<Material>()->spec() == ShaderSpec_Color;
		}
		return false;
	}
}