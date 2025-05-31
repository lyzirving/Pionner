#pragma once

#include "CoreDef.h"
#include "UUID.h"

#include <rttr/rttr_enable.h>
#include <rttr/type.h>

namespace pio
{
	class Object : public std::enable_shared_from_this<Object>
	{
		RTTR_ENABLE()
	public:
		Object() {}
		virtual ~Object() = default;
		virtual std::string ToString() const;

		uint32_t UId() const { return m_UID; }
		template<typename T = Object>
		Ref<T> Self() { return RefCast<Object, T>(shared_from_this()); }

		std::string GetClassName() const { return rttr::type::get(*this).get_name().to_string(); }

	public:
		template<typename T>
		bool Is() const 
		{ 
			auto thisType = rttr::type::get(*this);				
			return thisType == rttr::type::get<T>() ||
				   thisType.is_derived_from<T>();
		}

		template<typename T>
		const T* As() const { if(Is<T>()) { return static_cast<const T*>(this); } else { return nullptr; } }

		template<typename T>
		T* As() { if(Is<T>()) { return static_cast<T*>(this); } else { return nullptr; } }

	protected:
		UUID32 m_UID{};
	};
}