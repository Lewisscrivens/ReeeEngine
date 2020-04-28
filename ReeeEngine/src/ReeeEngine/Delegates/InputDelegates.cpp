#include "InputDelegates.h"
#include "../Windows/WindowsInput.h"

namespace ReeeEngine
{
	MouseMovedDelegate::MouseMovedDelegate(int X, int Y)
	{
		mousePosition = Vector2D(X, Y);
	}

	Vector2D MouseMovedDelegate::GetMousePos()
	{
		return mousePosition;
	}

	std::string MouseMovedDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Mouse moved to: " << mousePosition.ToString().c_str();
		return newString.str();
	}

	MouseScrolledDelegate::MouseScrolledDelegate(float delta)
	{
		scrollDelta = delta;
	}

	float MouseScrolledDelegate::GetScrollDelta()
	{
		return scrollDelta;
	}

	std::string MouseScrolledDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Mouse scrolled with current delta: " << scrollDelta;
		return newString.str();
	}

	MouseButtonDelegate::MouseButtonDelegate(EMouseKey mouseButton)
	{
		button = mouseButton;
	}

	EMouseKey MouseButtonDelegate::GetButton()
	{
		return button;
	}

	std::string MousePressedDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Button pressed: " << GetButtonString(button);
		return newString.str();
	}

	std::string MouseReleasedDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Button released: " << GetButtonString(button);
		return newString.str();
	}

	KeyDelegate::KeyDelegate(int keyCode)
	{
		currKeyCode = keyCode;
	}

	int KeyDelegate::GetKeyCode() const
	{
		return currKeyCode;
	}

	KeyPressedDelegate::KeyPressedDelegate(int keyCode, int repeatCount) : KeyDelegate(keyCode)
	{
		currRepeatCount = repeatCount;
	}

	int KeyPressedDelegate::GetRepeatCount() const
	{
		return currRepeatCount;
	}

	std::string KeyPressedDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Key pressed: " << currKeyCode
			      << " and repeated " << currRepeatCount << " times.";
		return newString.str();
	}

	KeyReleasedDelegate::KeyReleasedDelegate(int keyCode) : KeyDelegate(keyCode)
	{
		//...
	}

	std::string KeyReleasedDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Key released: " << currKeyCode;
		return newString.str();
	}

	std::string CharPressedDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Char typed: " << currKeyCode;
		return newString.str();
	}

	std::string KeyboardFocusLostDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Keyboard focus has been lost";
		return newString.str();
	}

	std::string MouseExitDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Mouse exited window.";
		return newString.str();
	}

	std::string MouseEnteredDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Mouse entered window.";
		return newString.str();
	}

}