#ifndef __PIONNER_UI_LAYER_LAYER_H__
#define __PIONNER_UI_LAYER_LAYER_H__

#include "ui/UiDef.h"

namespace pio
{
	class Asset;
	class Event;
	class Entity;
	class Registry;	

	class Layer
	{
	public:
		Layer(const LayoutParams &param, const std::string &name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual bool onEvent(Event &event) { return false; }
		// Ensure to be called in render thread
		virtual void onRenderDestroy() {}
		virtual void onUpdate(const Timestep &ts) {}
		virtual void onWindowSizeChange(uint32_t width, uint32_t height) {}

	public:
		inline const std::string &getName() const { return m_name; }
		inline const LayoutParams &getLayoutParam() const { return m_layoutParam; }

	protected:
		static Registry *s_registry;

	protected:
		std::string m_name{};
		LayoutParams m_layoutParam;
		UIEventTracker m_eventState{};
	};

}

#endif