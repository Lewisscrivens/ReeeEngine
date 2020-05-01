#pragma once
#include "ReeeMath.h"

namespace ReeeEngine
{
	/* 2D Vector deceleration. */
	struct Vector2D
	{
	public:

		/* Structure variables. */
		float X, Y;

	public:

		/* Default constructor's. */
		Vector2D() : X(0.0f), Y(0.0f) {}
		Vector2D(float val) : X(val), Y(val) {}
		Vector2D(int val) : X((float)val), Y((float)val) {}
		Vector2D(float x, float y) : X(x), Y(y) {}
		Vector2D(int x, int y) : X((float)x), Y((float)y) {}

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
		bool IsEqual(Vector2D otherVector, float tolerance = 0.0f)
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
		float DotProduct(const Vector2D& A, const Vector2D& B)
		{
			return A | B;
		}

		/* Return the cross product between two vectors. */
		float CrossProduct(const Vector2D& A, const Vector2D& B)
		{
			return A ^ B;
		}

		/* Returns absolute value of a vector 2d. */
		Vector2D Abs(Vector2D vector)
		{
			return Vector2D(std::abs(vector.X), std::abs(vector.Y));
		}

		/* Returns the normal of the given 2D Vector. */
		Vector2D GetNormal(float tolerance = 0.0f) const
		{
			const float vectorSizeSquared = SizeSquared();
			if (vectorSizeSquared > tolerance)
			{
				const float scale = ReeeMath::InvSqrt(vectorSizeSquared);
				return Vector2D(X * scale, Y * scale);
			}
			return Vector2D(0.0f, 0.0f);
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
		Vector2D Clamp(float min, float max)
		{
			return Vector2D(ReeeMath::Clamp(X, min, max), ReeeMath::Clamp(Y, min, max));
		}

		/* Returns the absolute value of this vector. */
		Vector2D Abs()
		{
			return Vector2D(std::abs(X), std::abs(Y));
		}

		/* Returns a string of this vector. */
		std::string ToString()
		{
			std::stringstream stream;
			stream << "Vector2D(X: " << X << ", Y: " << Y << ")";
			return stream.str();
		}

		/* Operator for setting one vector to another. */
		inline Vector2D& operator=(const Vector2D& otherVector)
		{
			X = otherVector.X;
			Y = otherVector.Y;
			return *this;
		}

		/* Override and setup equals operator. */
		inline bool operator==(const Vector2D& otherVector) const
		{
			if (otherVector.X == X && otherVector.Y == Y) return true;
			else return false;
		}

		/* Override and setup not equals operator. */
		inline bool operator!=(const Vector2D& otherVector) const
		{
			if (otherVector.X != X || otherVector.Y != Y) return true;
			else return false;
		}

		/* Override and setup add operator. */
		inline Vector2D operator+(const Vector2D& otherVector) const
		{
			return Vector2D(X + otherVector.X, Y + otherVector.Y);
		}

		/* Override and setup subtract operator. */
		inline Vector2D operator-(const Vector2D& otherVector) const
		{
			return Vector2D(X - otherVector.X, Y - otherVector.Y);
		}

		/* Override and setup negate operator. */
		inline Vector2D operator-() const
		{
			return Vector2D(-X, -Y);
		}

		/* Override and setup divide operator. */
		inline Vector2D operator/(const Vector2D& otherVector) const
		{
			return Vector2D(X / otherVector.X, Y / otherVector.Y);
		}

		/* Override and setup multiplication operator. */
		inline Vector2D operator*(const Vector2D& otherVector) const
		{
			return Vector2D(X * otherVector.X, Y * otherVector.Y);
		}

		/* Override the operator for mod. */
		inline float operator|(const Vector2D& otherVector) const
		{
			return X * otherVector.X + Y * otherVector.Y;
		}

		/* Override the power operator. */
		inline float operator^(const Vector2D& otherVector) const
		{
			return X * otherVector.Y - Y * otherVector.X;
		}

		/* Override the subtract float operator. */
		inline Vector2D operator-(float number) const
		{
			return Vector2D(X - number, Y - number);
		}

		/* Override the add float operator. */
		inline Vector2D operator+(float number) const
		{
			return Vector2D(X + number, Y + number);
		}

		/* Override less than operator. */
		inline bool operator<(const Vector2D& otherVector) const
		{
			return X < otherVector.X && Y < otherVector.Y;
		}

		/* Override greater than operator. */
		inline bool operator>(const Vector2D& otherVector) const
		{
			return X > otherVector.X && Y > otherVector.Y;
		}

		/* Override less than or equal to operator. */
		inline bool operator<=(const Vector2D& otherVector) const
		{
			return X <= otherVector.X && Y <= otherVector.Y;
		}

		/* Override greater than or equal operator. */
		inline bool operator>=(const Vector2D& otherVector) const
		{
			return X >= otherVector.X && Y >= otherVector.Y;
		}

		/* Overrides the plus equals operator. */
		inline Vector2D operator+=(const Vector2D& otherVector)
		{
			X += otherVector.X; Y += otherVector.Y;
			return *this;
		}

		/* Overrides the subtract equals operator. */
		inline Vector2D operator-=(const Vector2D& otherVector)
		{
			X -= otherVector.X;
			Y -= otherVector.Y;
			return *this;
		}

		/* Overrides the multiply equals float operator. */
		inline Vector2D operator*=(float number)
		{
			X *= number;
			Y *= number;
			return *this;
		}

		/* Overrides the divide equals float operator. */
		inline Vector2D operator/=(float number)
		{
			const float div = 1.f / number;
			X *= div;
			Y *= div;
			return *this;
		}

		/* Overrides the multiply equals 2D vector operator. */
		inline Vector2D operator*=(const Vector2D& otherVector)
		{
			X *= otherVector.X;
			Y *= otherVector.Y;
			return *this;
		}

		/* Overrides the divide equals 2D vector operator. */
		inline Vector2D operator/=(const Vector2D& otherVector)
		{
			X /= otherVector.X;
			Y /= otherVector.Y;
			return *this;
		}
	};
}