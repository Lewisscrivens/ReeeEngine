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
};

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

	/* Set the new X and Y value for the 2D vector. */
	void Set(float newX, float newY)
	{
		X = newX;
		Y = newX;
	}

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

	/* @Param tolerance, the tolerance to check from 0.
	 * @Returns if nearly equal to 0 with the given tolerance.
	 * True if the vector is zero, if parameter is > 0 then it will check with a tollerence. */
	bool IsZero(float tolerance = 0.0f)
	{
		if (tolerance != 0.0f)
		{
			float absoluteTolerance = std::abs(tolerance);
			return (X <= absoluteTolerance && X >= -absoluteTolerance &&
				Y <= absoluteTolerance && Y >= -absoluteTolerance);
		}
		else return (X == 0.0f && Y == 0.0f);
	}

	/* @Param tolerance, the tolerance to check if the vectors are near each other.
	 * @Returns if nearly equal to the other vector given the tolerance.
	 * Returns if the vector is nearly equal to another vector given a tolerance. */
	bool IsEqual(FVector2D otherVector, float tolerance = 0.0f)
	{
		if (tolerance != 0.0f)
		{
			float absoluteTolerance = std::abs(tolerance);
			return std::abs(X - otherVector.X) <= absoluteTolerance && std::abs(Y - otherVector.Y) <= absoluteTolerance;
		}
		else return (X == otherVector.X && Y == otherVector.Y);
	}

	/* Returns if the current values in the vector are valid floats true or false. */
	bool IsValid()
	{
		return ReeeMath::IsValid(X) && ReeeMath::IsValid(Y);
	}

	/* Returns if the current values in the vector are invalid true or false. */
	bool IsInvalid()
	{
		return !ReeeMath::IsValid(X) || !ReeeMath::IsValid(Y);
	}

	/* Return the dot product between two vectors. */
	float DotProduct(const FVector2D& A, const FVector2D& B)
	{
		return A | B;
	}

	/* Return the cross product between two vectors. */
	float CrossProduct(const FVector2D& A, const FVector2D& B)
	{
		return A ^ B;
	}

	/* Returns absolute value of a vector 2d. */
	FVector2D Abs(FVector2D vector)
	{
		return FVector2D(std::abs(vector.X), std::abs(vector.Y));
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

	/* Returns the clamp the vector between a min and max value across all axis. */
	FVector2D Clamp(float min, float max)
	{
		return FVector2D(ReeeMath::Clamp(X, min, max), ReeeMath::Clamp(Y, min, max));
	}

	/* Operator for setting one vector to another. */
	inline FVector2D& operator=(const FVector2D& otherVector)
	{
		X = otherVector.X;
		Y = otherVector.Y;
		return *this;
	}

	/* Override and setup equals operator. */
	inline bool operator==(const FVector2D& otherVector) const
	{
		if (otherVector.X == X && otherVector.Y == Y) return true;
		else return false;
	}

	/* Override and setup not equals operator. */
	inline bool operator!=(const FVector2D& otherVector) const
	{
		if (otherVector.X != X || otherVector.Y != Y) return true;
		else return false;
	}

	/* Override and setup add operator. */
	inline FVector2D operator+(const FVector2D& otherVector) const
	{
		return FVector2D(X + otherVector.X, Y + otherVector.Y);
	}

	/* Override and setup subtract operator. */
	inline FVector2D operator-(const FVector2D& otherVector) const
	{
		return FVector2D(X - otherVector.X, Y - otherVector.Y);
	}

	/* Override and setup negate operator. */
	inline FVector2D operator-() const
	{
		return FVector2D(-X, -Y);
	}

	/* Override and setup divide operator. */
	inline FVector2D operator/(const FVector2D& otherVector) const
	{
		return FVector2D(X / otherVector.X, Y / otherVector.Y);
	}

	/* Override and setup multiplication operator. */
	inline FVector2D operator*(const FVector2D& otherVector) const
	{
		return FVector2D(X * otherVector.X, Y * otherVector.Y);
	}

	/* Override the operator for mod. */
	inline float operator|(const FVector2D& otherVector) const
	{
		return X * otherVector.X + Y * otherVector.Y;
	}

	/* Override the power operator. */
	inline float operator^(const FVector2D& otherVector) const
	{
		return X * otherVector.Y - Y * otherVector.X;
	}

	/* Override the subtract float operator. */
	inline FVector2D operator-(float number) const
	{
		return FVector2D(X - number, Y - number);
	}

	/* Override the add float operator. */
	inline FVector2D operator+(float number) const
	{
		return FVector2D(X + number, Y + number);
	}

	/* Override less than operator. */
	inline bool operator<(const FVector2D& otherVector) const
	{
		return X < otherVector.X && Y < otherVector.Y;
	}

	/* Override greater than operator. */
	inline bool operator>(const FVector2D& otherVector) const
	{
		return X > otherVector.X && Y > otherVector.Y;
	}

	/* Override less than or equal to operator. */
	inline bool operator<=(const FVector2D& otherVector) const
	{
		return X <= otherVector.X && Y <= otherVector.Y;
	}

	/* Override greater than or equal operator. */
	inline bool operator>=(const FVector2D& otherVector) const
	{
		return X >= otherVector.X && Y >= otherVector.Y;
	}

	/* Overrides the plus equals operator. */
	inline FVector2D operator+=(const FVector2D& otherVector)
	{
		X += otherVector.X; Y += otherVector.Y;
		return *this;
	}

	/* Overrides the subtract equals operator. */
	inline FVector2D operator-=(const FVector2D& otherVector)
	{
		X -= otherVector.X;
		Y -= otherVector.Y;
		return *this;
	}

	/* Overrides the multiply equals float operator. */
	inline FVector2D operator*=(float number)
	{
		X *= number;
		Y *= number;
		return *this;
	}

	/* Overrides the divide equals float operator. */
	inline FVector2D operator/=(float number)
	{
		const float div = 1.f / number;
		X *= div;
		Y *= div;
		return *this;
	}

	/* Overrides the multiply equals 2D vector operator. */
	inline FVector2D operator*=(const FVector2D& otherVector)
	{
		X *= otherVector.X;
		Y *= otherVector.Y;
		return *this;
	}

	/* Overrides the divide equals 2D vector operator. */
	inline FVector2D operator/=(const FVector2D& otherVector)
	{
		X /= otherVector.X;
		Y /= otherVector.Y;
		return *this;
	}
};

/* 3D Vector deceleration. */
struct FVector3D
{
public:

	/* Structure variables. */
	float X, Y, Z;

public:

	/* Default constructor's. */
	FVector3D() : X(0.0f), Y(0.0f), Z(0.0f) {}
	FVector3D(float x, float y, float z) : X(x), Y(y), Z(z) {}

	////////////////////////////////////////////////
	/* Functions to help work with the FVector3D. */
	////////////////////////////////////////////////

	/* Set the new X, Y and Z value for the 3D vector. */
	void Set(float newX, float newY, float newZ)
	{
		X = newX;
		Y = newX;
		Z = newZ;
	}

	/* Returns the size of the vector. (Length.) */
	float Size() const
	{
		return std::sqrt(X * X + Y * Y + Z * Z);
	}

	/* Returns the size of the vector squared. (Length squared) */
	float SizeSquared() const
	{
		return X * X + Y * Y + Z * Z;
	}

	/* @Param tolerance, the tolerance to check from 0.
	 * @Returns if nearly equal to 0 with the given tolerance.
	 * True if the vector is zero, if parameter is > 0 then it will check with a tollerence. */
	bool IsZero(float tolerance = 0.0f)
	{
		if (tolerance != 0.0f)
		{
			float absoluteTolerance = std::abs(tolerance);
			return (X <= absoluteTolerance && X >= -absoluteTolerance &&
				Y <= absoluteTolerance && Y >= -absoluteTolerance &&
				Z <= absoluteTolerance && Z >= -absoluteTolerance);
		}
		else return (X == 0.0f && Y == 0.0f && Z == 0.0f);
	}

	/* @Param tolerance, the tolerance to check if the vectors are near each other.
	 * @Returns if nearly equal to the other vector given the tolerance.
	 * Returns if the vector is nearly equal to another vector given a tolerance. */
	bool IsEqual(FVector3D otherVector, float tolerance = 0.0f)
	{
		if (tolerance != 0.0f)
		{
			float absoluteTolerance = std::abs(tolerance);
			return std::abs(X - otherVector.X) <= absoluteTolerance
				&& std::abs(Y - otherVector.Y) <= absoluteTolerance
				&& std::abs(Z - otherVector.Z) <= absoluteTolerance;
		}
		else return (X == otherVector.X && Y == otherVector.Y && Z == otherVector.Z);
	}

	/* Returns if the current values in the vector are valid floats true or false. */
	bool IsValid()
	{
		return ReeeMath::IsValid(X) && ReeeMath::IsValid(Y) && ReeeMath::IsValid(Z);
	}

	/* Returns if the current values in the vector are invalid true or false. */
	bool IsInvalid()
	{
		return !ReeeMath::IsValid(X) || !ReeeMath::IsValid(Y) || !ReeeMath::IsValid(Z);
	}

	/* Return the dot product between two vectors. */
	float DotProduct(const FVector3D& A, const FVector3D& B)
	{
		return A | B;
	}

	/* Return the cross product between two vectors. */
	FVector3D CrossProduct(const FVector3D& A, const FVector3D& B)
	{
		return A ^ B;
	}

	/* Returns absolute value of a vector 2d. */
	FVector3D Abs(FVector3D vector)
	{
		return FVector3D(std::abs(vector.X), std::abs(vector.Y), std::abs(vector.Z));
	}

	/* Returns the normal of the given 3D Vector. */
	FVector3D GetNormal(float tolerance = 0.0f) const
	{
		const float vectorSizeSquared = SizeSquared();
		if (vectorSizeSquared > tolerance)
		{
			const float scale = ReeeMath::InvSqrt(vectorSizeSquared);
			return FVector3D(X * scale, Y * scale, Z * scale);
		}
		return FVector3D(0.0f, 0.0f, 0.0f);
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
			Z *= scale;
			return;
		}
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	/* Returns the clamp the vector between a min and max value across all axis. */
	FVector3D Clamp(float min, float max)
	{
		return FVector3D(ReeeMath::Clamp(X, min, max), ReeeMath::Clamp(Y, min, max), ReeeMath::Clamp(Z, min, max));
	}

	/* Override and setup equals operator. */
	inline bool operator==(const FVector3D& otherVector) const
	{
		if (otherVector.X == X && otherVector.Y == Y && otherVector.Z == Z) return true;
		else return false;
	}

	/* Override and setup not equals operator. */
	inline bool operator!=(const FVector3D& otherVector) const
	{
		if (otherVector.X != X || otherVector.Y != Y || otherVector.Z != Z) return true;
		else return false;
	}

	/* Override and setup add operator. */
	inline FVector3D operator+(const FVector3D& otherVector) const
	{
		return FVector3D(X + otherVector.X, Y + otherVector.Y, Z + otherVector.Z);
	}

	/* Override and setup subtract operator. */
	inline FVector3D operator-(const FVector3D& otherVector) const
	{
		return FVector3D(X - otherVector.X, Y - otherVector.Y, Z - otherVector.Z);
	}

	/* Override and setup negate operator. */
	inline FVector3D operator-() const
	{
		return FVector3D(-X, -Y, -Z);
	}

	/* Override and setup divide operator. */
	inline FVector3D operator/(const FVector3D& otherVector) const
	{
		return FVector3D(X / otherVector.X, Y / otherVector.Y, Z / otherVector.Z);
	}

	/* Override and setup multiplication operator. */
	inline FVector3D operator*(const FVector3D& otherVector) const
	{
		return FVector3D(X * otherVector.X, Y * otherVector.Y, Z * otherVector.Z);
	}

	/* Override the operator for mod. */
	inline float operator|(const FVector3D& otherVector) const
	{
		return X * otherVector.X + Y * otherVector.Y + Z * otherVector.Z;
	}

	/* Override the power operator. */
	inline FVector3D operator^(const FVector3D& otherVector) const
	{
		return FVector3D(Y * otherVector.Z - Z * otherVector.Y, Z * otherVector.X - X * otherVector.Z, X * otherVector.Y - Y * otherVector.X);
	}

	/* Override the subtract float operator. */
	inline FVector3D operator-(float number) const
	{
		return FVector3D(X - number, Y - number, Z - number);
	}

	/* Override the add float operator. */
	inline FVector3D operator+(float number) const
	{
		return FVector3D(X + number, Y + number, Z + number);
	}

	/* Override less than operator. */
	inline bool operator<(const FVector3D& otherVector) const
	{
		return X < otherVector.X && Y < otherVector.Y && Z < otherVector.Z;
	}

	/* Override greater than operator. */
	inline bool operator>(const FVector3D& otherVector) const
	{
		return X > otherVector.X && Y > otherVector.Y && Z > otherVector.Z;
	}

	/* Override less than or equal to operator. */
	inline bool operator<=(const FVector3D& otherVector) const
	{
		return X <= otherVector.X && Y <= otherVector.Y && Z <= otherVector.Z;
	}

	/* Override greater than or equal operator. */
	inline bool operator>=(const FVector3D& otherVector) const
	{
		return X >= otherVector.X && Y >= otherVector.Y && Z >= otherVector.Z;
	}

	/* Overrides the plus equals operator. */
	inline FVector3D operator+=(const FVector3D& otherVector)
	{
		X += otherVector.X;
		Y += otherVector.Y;
		Z += otherVector.Z;
		return *this;
	}

	/* Overrides the subtract equals operator. */
	inline FVector3D operator-=(const FVector3D& otherVector)
	{
		X -= otherVector.X;
		Y -= otherVector.Y;
		Z -= otherVector.Z;
		return *this;
	}

	/* Overrides the multiply equals float operator. */
	inline FVector3D operator*=(float number)
	{
		X *= number;
		Y *= number;
		Z *= number;
		return *this;
	}

	/* Overrides the divide equals float operator. */
	inline FVector3D operator/=(float number)
	{
		const float div = 1.f / number;
		X *= div;
		Y *= div;
		Z *= div;
		return *this;
	}

	/* Overrides the multiply equals 3D vector operator. */
	inline FVector3D operator*=(const FVector3D& otherVector)
	{
		X *= otherVector.X;
		Y *= otherVector.Y;
		Z *= otherVector.Z;
		return *this;
	}

	/* Overrides the divide equals 3D vector operator. */
	inline FVector3D operator/=(const FVector3D& otherVector)
	{
		X /= otherVector.X;
		Y /= otherVector.Y;
		Z /= otherVector.Z;
		return *this;
	}
};

