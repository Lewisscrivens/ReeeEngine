#pragma once
#include "ReeeMath.h"

namespace ReeeEngine
{
	/* 3D Vector deceleration. */
	struct Vector3D
	{
	public:

		/* Structure variables. */
		float X, Y, Z;

	public:

		/* Default constructor's. */
		Vector3D() : X(0.0f), Y(0.0f), Z(0.0f) {}
		Vector3D(float val) : X(val), Y(val), Z(val) {}
		Vector3D(float x, float y, float z) : X(x), Y(y), Z(z) {}

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

		/* Distance between this vector and other vector. */
		float Distance(Vector3D& otherVector)
		{
			return Abs(Vector3D(X - otherVector.X, Y - otherVector.Y, Z - otherVector.Z)).Size();
		}

		/* Returns the size of the vector. */
		float Size() const
		{
			return std::sqrt(X * X + Y * Y + Z * Z);
		}

		/* Returns the size of the vector squared. */
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
		bool IsEqual(Vector3D otherVector, float tolerance = 0.0f)
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
		float DotProduct(const Vector3D& A, const Vector3D& B)
		{
			return A | B;
		}

		/* Return the cross product between two vectors. */
		Vector3D CrossProduct(const Vector3D& A, const Vector3D& B)
		{
			return A ^ B;
		}

		/* Returns absolute value of a vector 2d. */
		Vector3D Abs(Vector3D vector)
		{
			return Vector3D(std::abs(vector.X), std::abs(vector.Y), std::abs(vector.Z));
		}

		/* Returns the normal of the given 3D Vector. */
		Vector3D GetNormal(float tolerance = 0.0f) const
		{
			const float vectorSizeSquared = SizeSquared();
			if (vectorSizeSquared > tolerance)
			{
				const float scale = ReeeMath::InvSqrt(vectorSizeSquared);
				return Vector3D(X * scale, Y * scale, Z * scale);
			}
			return Vector3D(0.0f, 0.0f, 0.0f);
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
		Vector3D Clamp(float min, float max)
		{
			return Vector3D(ReeeMath::Clamp(X, min, max), ReeeMath::Clamp(Y, min, max), ReeeMath::Clamp(Z, min, max));
		}

		/* Returns the absolute value of this vector. */
		Vector3D Abs()
		{
			return Vector3D(std::abs(X), std::abs(Y), std::abs(Z));
		}

		/* Returns a string of this vector. */
		std::string ToString()
		{
			std::stringstream stream;
			stream << "Vector3D(X: " << X << ", Y: " << Y << ", Z: " << Z << ")";
			return stream.str();
		}

		/* Return DirectX based XMFloat3 vector from this vector. */
		DirectX::XMFLOAT3 ToFloat3() const
		{
			return { X, Y, Z };
		}

		/* Override and setup equals operator. */
		inline bool operator==(const Vector3D& otherVector) const
		{
			if (otherVector.X == X && otherVector.Y == Y && otherVector.Z == Z) return true;
			else return false;
		}

		/* Override and setup not equals operator. */
		inline bool operator!=(const Vector3D& otherVector) const
		{
			if (otherVector.X != X || otherVector.Y != Y || otherVector.Z != Z) return true;
			else return false;
		}

		/* Override and setup add operator. */
		inline Vector3D operator+(const Vector3D& otherVector) const
		{
			return Vector3D(X + otherVector.X, Y + otherVector.Y, Z + otherVector.Z);
		}

		/* Override and setup subtract operator. */
		inline Vector3D operator-(const Vector3D& otherVector) const
		{
			return Vector3D(X - otherVector.X, Y - otherVector.Y, Z - otherVector.Z);
		}

		/* Override and setup negate operator. */
		inline Vector3D operator-() const
		{
			return Vector3D(-X, -Y, -Z);
		}

		/* Override and setup divide operator. */
		inline Vector3D operator/(const Vector3D& otherVector) const
		{
			return Vector3D(X / otherVector.X, Y / otherVector.Y, Z / otherVector.Z);
		}

		/* Override and setup multiplication operator. */
		inline Vector3D operator*(const Vector3D& otherVector) const
		{
			return Vector3D(X * otherVector.X, Y * otherVector.Y, Z * otherVector.Z);
		}

		/* Override the operator for mod. */
		inline float operator|(const Vector3D& otherVector) const
		{
			return X * otherVector.X + Y * otherVector.Y + Z * otherVector.Z;
		}

		/* Override the power operator. */
		inline Vector3D operator^(const Vector3D& otherVector) const
		{
			return Vector3D(Y * otherVector.Z - Z * otherVector.Y, Z * otherVector.X - X * otherVector.Z, X * otherVector.Y - Y * otherVector.X);
		}

		/* Override the subtract float operator. */
		inline Vector3D operator-(float number) const
		{
			return Vector3D(X - number, Y - number, Z - number);
		}

		/* Override the add float operator. */
		inline Vector3D operator+(float number) const
		{
			return Vector3D(X + number, Y + number, Z + number);
		}

		/* Override less than operator. */
		inline bool operator<(const Vector3D& otherVector) const
		{
			return X < otherVector.X && Y < otherVector.Y && Z < otherVector.Z;
		}

		/* Override greater than operator. */
		inline bool operator>(const Vector3D& otherVector) const
		{
			return X > otherVector.X && Y > otherVector.Y && Z > otherVector.Z;
		}

		/* Override less than or equal to operator. */
		inline bool operator<=(const Vector3D& otherVector) const
		{
			return X <= otherVector.X && Y <= otherVector.Y && Z <= otherVector.Z;
		}

		/* Override greater than or equal operator. */
		inline bool operator>=(const Vector3D& otherVector) const
		{
			return X >= otherVector.X && Y >= otherVector.Y && Z >= otherVector.Z;
		}

		/* Overrides the plus equals operator. */
		inline Vector3D operator+=(const Vector3D& otherVector)
		{
			X += otherVector.X;
			Y += otherVector.Y;
			Z += otherVector.Z;
			return *this;
		}

		/* Overrides the subtract equals operator. */
		inline Vector3D operator-=(const Vector3D& otherVector)
		{
			X -= otherVector.X;
			Y -= otherVector.Y;
			Z -= otherVector.Z;
			return *this;
		}

		/* Overrides the multiply equals float operator. */
		inline Vector3D operator*=(float number)
		{
			X *= number;
			Y *= number;
			Z *= number;
			return *this;
		}

		/* Overrides the divide equals float operator. */
		inline Vector3D operator/=(float number)
		{
			const float div = 1.f / number;
			X *= div;
			Y *= div;
			Z *= div;
			return *this;
		}

		/* Overrides the multiply equals 3D vector operator. */
		inline Vector3D operator*=(const Vector3D& otherVector)
		{
			X *= otherVector.X;
			Y *= otherVector.Y;
			Z *= otherVector.Z;
			return *this;
		}

		/* Overrides the divide equals 3D vector operator. */
		inline Vector3D operator/=(const Vector3D& otherVector)
		{
			X /= otherVector.X;
			Y /= otherVector.Y;
			Z /= otherVector.Z;
			return *this;
		}
	};
}