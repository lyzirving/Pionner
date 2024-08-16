#ifndef __PIONNER_EVENT_EB_DEF_H__
#define __PIONNER_EVENT_EB_DEF_H__

#include "core/Base.h"

namespace pio
{
	class Asset;

	/*
	* @brief EventBusObject should be the first parent class in class hierarchy
	*/
	class EventBusObject
	{
	public:
		EventBusObject() {}
		virtual ~EventBusObject() = default;
	};

	enum class PioEvent : uint8_t
	{
		None = 0, UnzipAsset
	};

	struct EventBusArg
	{
		PioEvent Event;
		Ref<Asset> Assets;

		EventBusArg() : Event(PioEvent::None), Assets() {}
		EventBusArg(PioEvent event) : Event(event) {}
		EventBusArg(PioEvent event, const Ref<Asset> &assets) : Event(event), Assets(assets) {}

		EventBusArg(const EventBusArg &rhs) : Event(rhs.Event), Assets(rhs.Assets) {}

		EventBusArg(EventBusArg &&rhs) noexcept
		{
			if (this != &rhs)
			{
				this->Event = rhs.Event;
				this->Assets = rhs.Assets;
				rhs.Event = PioEvent::None;
				rhs.Assets.reset();
			}
		}

		EventBusArg &operator=(const EventBusArg &rhs)
		{
			if (this != &rhs)
			{
				this->Event = rhs.Event;
				this->Assets = rhs.Assets;
			}
			return *this;
		}

		EventBusArg &operator=(EventBusArg &&rhs) noexcept
		{
			if (this != &rhs)
			{
				this->Event = rhs.Event;
				this->Assets = rhs.Assets;
				rhs.Event = PioEvent::None;
				rhs.Assets.reset();
			}
			return *this;
		}

		~EventBusArg() { Assets.reset(); }
	};

	using EventBusTaskFunc = std::function<void()>;
	using EventBusCbFunc = void (EventBusObject:: *)(const EventBusArg &);

	struct EventBusCb
	{
		EventBusObject *Target{ nullptr };
		EventBusCbFunc Cb{ nullptr };

		EventBusCb() {}

		EventBusCb(EventBusObject *target, EventBusCbFunc cb) : Target(target), Cb(cb) {}

		EventBusCb(const EventBusCb &rhs) : Target(rhs.Target), Cb(rhs.Cb) {}

		EventBusCb(EventBusCb &&rhs) noexcept
		{
			if (this != &rhs)
			{
				this->Target = rhs.Target;
				this->Cb = rhs.Cb;
				rhs.Target = nullptr;
				rhs.Cb = nullptr;
			}
		}

		EventBusCb &operator=(const EventBusCb &rhs)
		{
			if (this != &rhs)
			{
				this->Target = rhs.Target;
				this->Cb = rhs.Cb;
			}
			return *this;
		}

		EventBusCb &operator=(EventBusCb &&rhs) noexcept
		{
			if (this != &rhs)
			{
				this->Target = rhs.Target;
				this->Cb = rhs.Cb;
				rhs.Target = nullptr;
				rhs.Cb = nullptr;
			}
			return *this;
		}

		bool operator==(const EventBusCb &rhs)
		{
			return this->Target == rhs.Target && this->Cb == rhs.Cb;
		}
	};
}

#endif