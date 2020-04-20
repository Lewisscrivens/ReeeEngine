#pragma once

/* Include api. */
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

/* Include common types. */
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

/* Platform definitions. */
#ifdef PLATFORM_WINDOWS
	#ifdef REEE_BUILD
		#define REEE_API __declspec(dllexport)
	#else
		#define REEE_API __declspec(dllimport)
	#endif
#else 
	#error Windows only!!!
#endif

/* Engine debugging macros. */
#define DEBUG_ENABLED 1;

/* Reference to pointer functions. */
namespace ReeeEngine
{
	template<typename T>
	using Pointer = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Pointer<T> CreatePointer(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}