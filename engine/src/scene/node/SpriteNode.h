#pragma once

#include "StaticMeshNode.h"

namespace pio
{
	class Image;

	class SpriteNode : public StaticMeshNode
	{
		RTTR_ENABLE(StaticMeshNode)
	public:
		SpriteNode() : StaticMeshNode() {}
		virtual ~SpriteNode() = default;

		void SetFlipY(bool flip);
		void SetImage(const Ref<Image>& image);		

	protected:
		virtual void OnInit() override;
		virtual void OnTickComponents(const Ref<RenderContext>& context) override;
		virtual void OnTick(const Ref<RenderContext>& context) override { /* should override and do nothing */}
	};
}