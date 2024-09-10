#ifndef __PIONNER_UI_RUNTIME_LAYER_H__
#define __PIONNER_UI_RUNTIME_LAYER_H__

#include "Layer.h"

namespace pio
{
	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer(const LayoutParams& param, const std::string& name = "Layer");
		~RuntimeLayer() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onDetach() override;
		virtual void onUpdate(Ref<RenderContext>& context, Ref<RenderTarget>& target) override;

	private:
		bool m_firstTimeShow{ true };
	};
}

#endif