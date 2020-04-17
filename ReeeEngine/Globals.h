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

/* Include global engine classes. */
#include "ReeeLog.h"

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