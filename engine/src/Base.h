#ifndef __PIONNER_BASE_H__
#define __PIONNER_BASE_H__

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <list>
#include <tuple>

#include <string>
#include <string_view>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <initializer_list>
#include <memory>
#include <functional>
#include <type_traits>

#include <algorithm>
#include <cmath>
#include <bitset>

#include <typeinfo>
#include <cassert>

#include "base/Types.h"
#include "base/UUID.h"
#include "base/QuatUtil.h"
#include "base/utils/Time.h"
#include "base/utils/LogSystem.h"

#include "magic_enum.hpp"

namespace pio
{
	constexpr static const uint32_t InvalidId = 0U;

	using byte = uint8_t;

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename Parent, typename Derived, typename ... Args>
	constexpr Ref<Parent> CreateRef(Args&& ... args)
	{
		return std::shared_ptr<Parent>(new Derived(std::forward<Args>(args)...));
	}

	template<typename Parent, typename Derived>
	constexpr Ref<Derived> RefCast(const Ref<Parent> &p)
	{
		Ref<Derived> ret{ nullptr };
		if (std::is_base_of<Parent, Derived>())
			ret = std::dynamic_pointer_cast<Derived>(p);
		return ret;
	}

	namespace Base
	{
		/*
		 * This method should be used with PIO_TO_STR, for example
		 * auto name = GetMemberName(#PIO_TO_STR(m_lightEnv.DirectionalLight));
		 * The result name is "DirectionalLight".
		*/
		inline std::string GetMemberName(const char* exp)
		{
			std::string ret(exp);
			std::size_t pos = ret.find_last_of(".");
			if (pos != std::string::npos)
				return ret.substr(pos + 1);

			pos = ret.find_last_of("->");
			if (pos != std::string::npos)
				return ret.substr(pos + 1);

			return "invalid expression!";
		}
	}

	#define PIO_MALLOC(num) std::malloc(num)

	#define PIO_FREE(p)  if(p) { std::free(p); p = nullptr; }

	#define PIO_TO_STR(x) #x

	// TODO: num of bone in some model might be greater than PIO_MAX_BONE_MATRIX, 
	//       how to deal with this case?
	#define PIO_MAX_BONE_MATRIX (100)

	#define PIO_MAX_BONE_INFLUENCE (4)

	#define PIO_BIT(x) (1 << (x))

	// Size of a static C-style array. Don't use on pointers!
	#define PIO_ARRAY_SIZE(ARR) ((int)(sizeof(ARR) / sizeof(*(ARR))))

	#define PIO_ASSERT(check, fmt, ...)   { if(!(check)) { LOGE(fmt, __VA_ARGS__) } }
	#define PIO_ASSERT_RETURN(check, fmt, ...) { if(!(check)) { LOGE(fmt, __VA_ARGS__) return; } }
	#define PIO_ASSERT_RETURN_FALSE(check, fmt, ...) { if(!(check)) { LOGE(fmt, __VA_ARGS__) return false; } }
	#define PIO_ASSERT_CONTINUE(check, fmt, ...) { if(!(check)) { LOGW(fmt, __VA_ARGS__) continue; } }

	// Fill __VA_ARGS__ with std::placeholders::_1, std::placeholders::_2 or std::placeholders::_N 
	// referencing to the param num of function
	#define PIO_BIND_FN_SELF(fn, ...)         std::bind(&fn, this, ##__VA_ARGS__)
	#define PIO_BIND_FN_OTHER(fn, who, ...)   std::bind(&fn, who,  ##__VA_ARGS__)

	#define PIO_CHECK_EVT_HANDLE_AND_RETURN(evt) if(evt->Handled) {return;}
	#define PIO_CHECK_EVT_HANDLE_AND_BREAK(evt) if(evt->Handled) {break;}

	#define PIO_UINT8(x) uint8_t(x)
	#define PIO_UINT(x)  uint32_t(x)
	#define PIO_INT(x)   int32_t(x)

	#define PIO_VOID_PTR_TO_PRIMITIVE(Ptr, Type)  (Type)(*((Type *)(&(Ptr))))

	#define PIO_SINGLETON_DECLARE(T)	public:\
										static T* Get();\
                                        static void Init();\
                                        static void Shutdown();\
										private:\
										T();\
										~T();\
										private:\
										static T *s_instance;\
										static std::mutex s_mutex;
	
	#define PIO_SINGLETON_IMPL(T)		T *T::s_instance{nullptr};\
                                        std::mutex T::s_mutex{};\
										T *T::Get() {\
											if(s_instance == nullptr) {\
												std::lock_guard<std::mutex> lock{s_mutex};\
												if(s_instance == nullptr) {\
													s_instance = new T;\
												}\
											}\
											return s_instance;\
										}

	// need to include Uniform.h
	#define PIO_UNIFORM_STRUCT_DECLARE(T)	UniformBlock Block;\
											void obtainBlock() { this->Block = T::CreateBlock(); }\
											void serialize();\
                                            static UniformBlock CreateBlock();
}

#endif