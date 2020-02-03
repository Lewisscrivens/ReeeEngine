#pragma once
#include <cmath>
#include <DirectXMath.h>

/* Will contain any extra classes or structures that are easier to use or not included with DirectXMath.h */
class ReeeMath
{
public:

	/* Delete Constructors. */
	ReeeMath() = default;
	ReeeMath(const ReeeMath&) = delete;
	ReeeMath& operator = (const ReeeMath&) = delete;

public:

	/* 2D Vector deceleration. */
	struct FVector2D 
	{
	public:

		/* Structure variables. */
		float X, Y;

	public:

		/* Default constructor's. */
		FVector2D() : X(0.0f), Y(0.0f) {}
		FVector2D(float x, float y) : X(x), Y(y) {}

		////////////////////////////////////////////////
		/* Functions to help work with the FVector2D. */
		////////////////////////////////////////////////

		/* Returns the size of the vector. (Length.) */
		float Size() const
		{
			return std::sqrt(X * X + Y * Y);
		}

		/* Returns the size of the vector squared. (Length squared) */
		float SizeSquared() const
		{
			return X * X + Y * Y;
		}

		/* Returns the normal of the given 2D Vector. */
		FVector2D GetNormal(float tolerance = 0.0f) const
		{
			const float vectorSizeSquared = SizeSquared();
			if (vectorSizeSquared > tolerance)
			{
				const float scale = ReeeMath::InvSqrt(vectorSizeSquared);
				return FVector2D(X * scale, Y * scale);
			}
			return FVector2D(0.0f, 0.0f);
		}

		/* Normalize the 2D vector. */
		void Normalize(float tolerance = 0.0f)
		{
			const float vectorSizeSquared = SizeSquared();
			if (vectorSizeSquared > tolerance)
			{
				const float scale = ReeeMath::InvSqrt(vectorSizeSquared);
				X *= scale;
				Y *= scale;
				return;
			}
			X = 0.0f;
			Y = 0.0f;
		}

		/* Override and setup equals operator. */
		inline bool operator==(FVector2D otherVector) 
		{
			if (otherVector.X == X && otherVector.Y == Y) return true;
			else return false;
		}

		/* Override and setup not equals operator. */
		inline bool operator!=(FVector2D otherVector)
		{
			if (otherVector.X != X || otherVector.Y != Y) return true;
			else return false;
		}



	};

	/* 3D Vector deceleration. */
	struct FVector2D
	{

	};

public:

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
};

