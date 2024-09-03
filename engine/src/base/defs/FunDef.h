#ifndef __PIONNER_BASE_DEFS_FUN_DEF_H__
#define __PIONNER_BASE_DEFS_FUN_DEF_H__

#include <memory>

#include <thread>
#include <functional>

#include <mutex>
#include <condition_variable>
#include <atomic>

#include <map>
#include <set>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <string>
#include <string_view>
#include <sstream>

#include <initializer_list>

#include <tuple>
#include <bitset>

#include <type_traits>
#include <typeinfo>
#include <cassert>

namespace pio
{
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
	constexpr Ref<Derived> RefCast(const Ref<Parent>& p)
	{
		Ref<Derived> ret{ nullptr };
		if (std::is_base_of<Parent, Derived>())
			ret = std::dynamic_pointer_cast<Derived>(p);
		return ret;
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
	
	#define PIO_CHECK_RETURN(check, fmt, ...) { if(!(check)) { LOGE(fmt, __VA_ARGS__) return; } }
	#define PIO_CHECK_RETURN_FALSE(check, fmt, ...) { if(!(check)) { LOGE(fmt, __VA_ARGS__) return false; } }
	#define PIO_CHECK_CONTINUE(check, fmt, ...) { if(!(check)) { LOGW(fmt, __VA_ARGS__) continue; } }

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

	// need to include UniformPack.h
	#define PIO_UNIFORM_STRUCT_DECLARE(T)	UniformBlock Block;\
											void obtainBlock() { this->Block = T::CreateBlock(); }\
											void serialize();\
	                                        static UniformBlock CreateBlock();
}

#endif