#pragma once
#include "Delegate.h"
#include "../Math/Vector2D.h"
#include "../Windows/WindowsKeyCodes.h"

namespace ReeeEngine
{
	/* Define delegate for mouse being moved. */
	class REEE_API MouseMovedDelegate : public Delegate
	{
	public:

		// Setup for a new delegate call when the mouse is moved.
		MouseMovedDelegate(Vector2D mousePos);

		// Return the delegate type.
		virtual DelegateType GetType() const override { return DelegateType::Input; };

		// Get the current mouse position for the following event/delegate call.
		Vector2D GetMousePos();

		// Return description of delegate event.
		std::string ToString() override;

	private:
		
		// The delegates mouse position on called.
		Vector2D mousePosition;
	};

	/* Define delegate for mouse being scrolled. */
	class REEE_API MouseScrolledDelegate : public Delegate
	{
	public:

		// Setup for a new delegate call when the mouse is scrolled.
		MouseScrolledDelegate(float delta);

		// Return the delegate type.
		virtual DelegateType GetType() const override { return DelegateType::Input; };

		// Get the current mouse scroll delta for the following event/delegate call.
		float GetScrollDelta();

		// Return description of delegate event.
		std::string ToString() override;

	private:

		// The delegates scrollDelta when called.
		float scrollDelta;
	};

	/* Define mouse button types. */
	enum class EMouseKey
	{
		Left,
		Right,
		Middle
	};

	/* Delegate for the a mouse button being pressed. */
	class REEE_API MouseButtonDelegate : public Delegate
	{
	protected:

		// Setup for a new delegate call when a mouse button is pressed.
		MouseButtonDelegate(EMouseKey mouseButton);

		// Return string for mouse button.
		static std::string GetButtonString(EMouseKey mouseButton)
		{
			std::string buttonString = "";
			switch (mouseButton)
			{
			case ReeeEngine::EMouseKey::Left:
				buttonString += "Left";
				break;
			case ReeeEngine::EMouseKey::Right:
				buttonString += "Right";
				break;
			case ReeeEngine::EMouseKey::Middle:
				buttonString += "Middle";
				break;
			}

			// Return converted string.
			return buttonString;
		}

		// The delegates mouse button type.
		EMouseKey button;

	public:

		// Return the delegate type.
		virtual DelegateType GetType() const override { return DelegateType::Input; };

		// Get the mouse button that was pressed.
		EMouseKey GetButton();
	};

	/* Delegate for the a mouse button being pressed. */
	class REEE_API MousePressedDelegate : public MouseButtonDelegate
	{
	public:

		// Return description of delegate event.
		std::string ToString() override;
	};

	/* Delegate for the a mouse button being released. */
	class REEE_API MouseReleasedDelegate : public MouseButtonDelegate
	{
	public:

		// Return description of delegate event.
		std::string ToString() override;
	};

	/* Delegate base class for keyboard input. */
	class REEE_API KeyDelegate : public Delegate
	{
	public:

		// Return the key code of the key pressed/released etc.
		int GetKeyCode() const;

		// Return the delegate type.
		virtual DelegateType GetType() const override { return DelegateType::Input; };

	protected:

		// Use default constructor.
		KeyDelegate() = default;

		// Default key event constructor for setting key code when new delegate is created.
		KeyDelegate(int keyCode);

	protected:

		// The current key code for the new delegate.
		int currKeyCode;
	};

	/* Delegate for when a keyboard key is pressed and/or held. */
	class REEE_API KeyPressedDelegate : public KeyDelegate
	{
	public:

		// Initalise both the key code and repeat count.
		KeyPressedDelegate(int keyCode, int repeatCount);

		// Get the repeat count of the current key delegate.
		int GetRepeatCount() const;

		// Return description of delegate event.
	    std::string ToString() override;

	private:

		// Current repeated run of this keyCodes pressed delegate.
		int currRepeatCount;
	};

	/* Delegate for when a keyboard key is released. */
	class REEE_API KeyReleasedDelegate : public KeyDelegate
	{
	public:

		// Initalise both the key code for the delegate.
		KeyReleasedDelegate(int keyCode);

		// Return description of delegate event.
		std::string ToString() override;
	};

	/* Delegate for when a keyboard character is pressed. */
	class REEE_API CharPressedDelegate : public KeyDelegate
	{
	public:

		// Use default constructor to set current key code on init.
		CharPressedDelegate(int KeyCode) : KeyDelegate(KeyCode) {}

		// Return description of delegate event.
		std::string ToString() override;
	};
}




