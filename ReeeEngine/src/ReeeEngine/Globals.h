#pragma once

/* Include api. */
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <optional>

/* Include common types. */
#include <string>
#include <sstream>
#include <ostream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

/* Include windows. */
#include "ReeeEngine/Windows/ReeeWin.h"

/* Useful macros. */
#define CHECK_RETURN(input) if (!input) { return; } ;

/* Is profiling enabled? */
#define PROFILING 0

/* Platform definitions. */
#ifdef PLATFORM_WINDOWS
	// Define Engine API.
	#ifdef REEE_BUILD
		#define REEE_API __declspec(dllexport)
	#else
		#define REEE_API __declspec(dllimport)
	#endif
#else 
	#error Windows only!!!
#endif

/* Reference to pointer and reference functions. */
namespace ReeeEngine
{
	template<typename T>
	using Refference = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Refference<T> CreateReff(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Pointer = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Pointer<T> CreatePointer(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}