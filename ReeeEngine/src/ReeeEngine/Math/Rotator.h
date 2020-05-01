#pragma once
#include "ReeeMath.h"

namespace ReeeEngine
{
	/* Rotator declaration. */
	struct Rotator
	{
	public:

		/* Structure variables. */
		float Pitch, Yaw, Roll;

	public:

		/* Default constructor's. */
		Rotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {}
		Rotator(float val) : Pitch(val), Yaw(val), Roll(val) {}
		Rotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}

		//////////////////////////////////////////////////////////////
		//					  Rotator Functions						//
		//////////////////////////////////////////////////////////////

		/* Returns the absolute value of this rotator. */
		Rotator Abs() const
		{
			return Rotator(std::abs(Pitch), std::abs(Yaw), std::abs(Roll));
		}

		/* Returns absolute value of a given rotator. */
		Rotator Abs(Rotator rotator) const
		{
			return Rotator(std::abs(rotator.Pitch), std::abs(rotator.Yaw), std::abs(rotator.Roll));
		}

		/* Return this rotator in radian values for pitch, yaw and roll. NOTE: Assumes its in degrees... */
		Rotator ToRadians() const
		{
			return Rotator(Pitch * (PI / 180.0f), Yaw * (PI / 180.0f), Roll * (PI / 180.0f));
		}

		/* Converts this rotator back to degrees. NOTE: Assumes its in radians... */
		Rotator ToDegrees()
		{
			return Rotator((Pitch * 180.0f) / PI, (Yaw * 180.0f) / PI, (Roll * 180.0f) / PI);
		}

		/* Clamp a given angle between 0 and 360 */
		float ClampAngle(float val) const
		{
			val = std::fmod(val, 360.f);
			if (val < 0.0f) val += 360.f;
			return val;
		}

		/* Normalize this rotator. */
		void Normalize()
		{
			Pitch = NormalizeAngle(Pitch);
			Yaw = NormalizeAngle(Yaw);
			Roll = NormalizeAngle(Roll);
		}

		/* Return a normalized version of this rotator. */
		Rotator GetNormalized() const
		{
			return Rotator(NormalizeAngle(Pitch), NormalizeAngle(Yaw), NormalizeAngle(Roll));
		}

		/* Normalizes a given value of the rotator. */
		float NormalizeAngle(float Angle) const
		{
			// Clamp between 0 and 360.0f.
			Angle = ClampAngle(Angle);
			if (Angle > 180.f) Angle -= 360.f;
			return Angle;
		}

		/* Returns true or false on weather the rotator is zero. To check with tolerance use equals function. */
		bool IsZero() const
		{
			return (ClampAngle(Pitch) == 0.0f) && (ClampAngle(Yaw) == 0.0f) && (ClampAngle(Roll) == 0.0f);
		}

		/* Checks if a rotator is equal to another with an optional tolerance. */
		bool Equals(const Rotator& otherRotator, float tolerance = 0.0f) const
		{
			return (ReeeMath::Abs(NormalizeAngle(Pitch - otherRotator.Pitch)) <= tolerance)
				&& (ReeeMath::Abs(NormalizeAngle(Yaw - otherRotator.Yaw)) <= tolerance)
				&& (ReeeMath::Abs(NormalizeAngle(Roll - otherRotator.Roll)) <= tolerance);
		}

		/* Returns if the current values in the rotator are valid floats true or false. */
		bool IsValid() const
		{
			return ReeeMath::IsValid(Pitch) && ReeeMath::IsValid(Yaw) && ReeeMath::IsValid(Roll);
		}

		/* Returns if the current values in the rotator are invalid true or false. */
		bool IsInvalid() const
		{
			return !ReeeMath::IsValid(Pitch) || !ReeeMath::IsValid(Yaw) || !ReeeMath::IsValid(Roll);
		}

		/* Return a string describing this rotator. */
		std::string ToString() const
		{
			std::stringstream stream;
			stream << "Rotator(Pitch: " << Pitch << ", Yaw: " << Yaw << ", Roll: " << Roll << ")";
			return stream.str();
		}

		//////////////////////////////////////////////////////////////
		//					  Operator Setups						//
		//////////////////////////////////////////////////////////////

		/* Override and setup equals check operator. */
		bool operator==(const Rotator& otherRotator) const
		{
			return (Pitch == otherRotator.Pitch && Yaw == otherRotator.Yaw && Roll == otherRotator.Roll);
		}

		/* Override and setup doesn't equal check operator. */
		bool operator!=(const Rotator& otherRotator) const
		{
			return (Pitch != otherRotator.Pitch || Yaw != otherRotator.Yaw || Roll != otherRotator.Roll);
		}

		/* Override less than operator. */
		bool operator<(const Rotator& otherRotator) const
		{
			return Pitch < otherRotator.Pitch && Yaw < otherRotator.Yaw && Roll < otherRotator.Roll;
		}

		/* Override and setup add-equals rotator operator. */
		Rotator operator+=(const Rotator& otherRotator)
		{
			Pitch = Pitch * otherRotator.Pitch;
			Yaw = Yaw * otherRotator.Yaw;
			Roll = Roll * otherRotator.Roll;
			return *this;
		}

		/* Override and setup subtract-equals rotator operator. */
		Rotator operator-=(const Rotator& otherRotator)
		{
			Pitch = Pitch - otherRotator.Pitch;
			Yaw = Yaw - otherRotator.Yaw;
			Roll = Roll - otherRotator.Roll;
			return *this;
		}

		/* Override and setup multiply-equals rotator operator. */
		Rotator operator*=(const Rotator& otherRotator)
		{
			Pitch = Pitch * otherRotator.Pitch;
			Yaw = Yaw * otherRotator.Yaw;
			Roll = Roll * otherRotator.Roll;
			return *this;
		}

		/* Override and setup add by rotator operator. */
		Rotator operator+(const Rotator& otherRotator) const
		{
			return Rotator(Pitch + otherRotator.Pitch, Yaw + otherRotator.Yaw, Roll + otherRotator.Roll);
		}

		/* Override and setup subtract by rotator operator. */
		Rotator operator-(const Rotator& otherRotator) const
		{
			return Rotator(Pitch - otherRotator.Pitch, Yaw - otherRotator.Yaw, Roll - otherRotator.Roll);
		}

		/* Override and setup multiply by rotator operator. */
		Rotator operator*(const Rotator& otherRotator) const
		{
			return Rotator(Pitch * otherRotator.Pitch, Yaw * otherRotator.Yaw, Roll * otherRotator.Roll);
		}

		/* Override and setup add by value operator. */
		Rotator operator+(float val) const
		{
			return Rotator(Pitch + val, Yaw + val, Roll + val);
		}

		/* Override and setup subtract by value operator. */
		Rotator operator-(float val) const
		{
			return Rotator(Pitch - val, Yaw - val, Roll - val);
		}

		/* Override and setup multiply by value operator. */
		Rotator operator*(float val) const
		{
			return Rotator(Pitch * val, Yaw * val, Roll * val);
		}

		/* Override and setup multiply-equals value operator. */
		Rotator operator*=(float val)
		{
			Pitch = Pitch * val; 
			Yaw = Yaw * val; 
			Roll = Roll * val;
			return *this;
		}

		/* Override and setup add-equals value operator. */
		Rotator operator+=(float val)
		{
			Pitch = Pitch + val;
			Yaw = Yaw + val;
			Roll = Roll + val;
			return *this;
		}

		/* Override and setup subtract-equals value operator. */
		Rotator operator-=(float val)
		{
			Pitch = Pitch - val;
			Yaw = Yaw - val;
			Roll = Roll - val;
			return *this;
		}
	};
}