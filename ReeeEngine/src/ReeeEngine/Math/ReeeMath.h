#pragma once
#include "../Globals.h"
#include <cmath>
#include <DirectXMath.h>

namespace ReeeEngine
{
	/* Define common numbers. */
	#define PI 3.14159265f 
    #define PID 3.1415926535897932

	/* Will contain any extra classes or structures that are easier to use or not included with DirectXMath.h */
	class ReeeMath
	{
	public:

		/* Delete Constructors. */
		ReeeMath() = default;
		ReeeMath(const ReeeMath&) = delete;
		ReeeMath& operator = (const ReeeMath&) = delete;

	public:

		/* Squared function. */
		template <typename T>
		static T Sqrt(const T& val)
		{
			return val * val;
		}

		/* Function for getting the inverse square root of a given value.
		 * NOTE: Re-purposed code from Quake that was released through wiki. */
		static float InvSqrt(float val)
		{
			const __m128 fOneHalf = _mm_set_ss(0.5f);
			__m128 y0, x0, x1, x2, fOver2;
			float temp;

			y0 = _mm_set_ss(val);
			x0 = _mm_rsqrt_ss(y0);	// Estimate 1 / sqrt at 12 bits.
			fOver2 = _mm_mul_ss(y0, fOneHalf);

			// 1st Newton-Raphson iteration
			x1 = _mm_mul_ss(x0, x0);
			x1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(fOver2, x1));
			x1 = _mm_add_ss(x0, _mm_mul_ss(x0, x1));

			// 2nd Newton-Raphson iteration
			x2 = _mm_mul_ss(x1, x1);
			x2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(fOver2, x2));
			x2 = _mm_add_ss(x1, _mm_mul_ss(x1, x2));

			// Return the Inverse Square root of the given value.
			_mm_store_ss(&temp, x2);
			return temp;
		}

		/* Convert floating point degrees value to radians. */
		static float Radians(const float degrees)
		{
			return (degrees * (PI/180.0f));
		}

		/* Return a given aspect ratio from a new screen width/height. */
		static float GetAspectRatio(const float width, const float height)
		{
			return ((float)width / height);
		}

		/* Clamp a given float value between two points. */
		template<class T>
		static T Clamp(T value, T min, T max)
		{
			return (value < min) ? min : (max < value) ? max : value;
		}

		/** Return true if value is not infinite and a valid floating point value. */
		static inline bool IsValid(float value)
		{
			return !isnan(value) && isfinite(value);
		}

		/* Absolute function for a float. */
		static inline float Abs(float value)
		{
			return std::abs(value);
		}
	};
}