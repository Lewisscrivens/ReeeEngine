#pragma once
#include <queue>
#include <bitset>
#include "../Math/Vector2D.h"
#include "WindowsKeyCodes.h"

namespace ReeeEngine
{
	/* Declare mouse and keyboard input macros for disabling input functionality for both. */
	#define KEYBOARD_ENABLED 1;
	#define MOUSE_ENABLED 1;

	/* Class to receive and manage input from the keyboard through the window class. Each window has a window input and graphics class. */
	class REEE_API WindowsInput
	{
		friend class Window;

	public:

		/* Event class to assign events to messages related to the keyboard. */
		class KeyboardEvent
		{
		public:

			/* Enum for type of event being called. */
			enum class Type
			{
				Down,
				Up,
				Invalid
			};

		private:

			/* Declare event vars. */
			Type eventType;
			unsigned char eventCode;
			int eventCount; // Key count of how many times its been ran while held.

		public:

			/* Default constructors. */
			KeyboardEvent() : eventType(Type::Invalid), eventCode(0u) {}
			KeyboardEvent(Type type, unsigned char code, int keyCount) noexcept : eventType(type), eventCode(code), eventCount (keyCount) {}

			/* Functions to check event type / state. */
			bool IsDown() const noexcept { return eventType == Type::Down; }
			bool IsUp() const noexcept { return eventType == Type::Up; }
			bool IsInvalid() const noexcept { return eventType == Type::Invalid; }

			/* Returns the current event code. */
			unsigned char GetEventCode() const noexcept { return eventCode; }
		};

		/* Event class to assign events to messages related to the mouse. */
		class MouseEvent
		{
		public:

			/* Enum for type of event being called. */
			enum class Type
			{
				LeftDown,
				LeftUp,
				RightDown,
				RightUp,
				MiddleDown,
				MiddleUp,
				ScrollUp,
				ScrollDown,
				Move,
				InsideWindow,
				OutsideWindow,
				Invalid
			};

		private:

			/* Declare event variables for handling mouse input. */
			Type eventType;
			bool leftDown, rightDown, middleDown;
			Vector2D mousePosition;

		public:

			/* Default constructors for the mouse event. */
			MouseEvent() : eventType(Type::Invalid), leftDown(false), rightDown(false), middleDown(false), mousePosition(Vector2D(0.0f, 0.0f)) {}
			MouseEvent(Type type, const WindowsInput& parent) noexcept : eventType(type), leftDown(parent.leftMouseDown), rightDown(parent.rightMouseDown), middleDown(parent.middleMouseDown), mousePosition(Vector2D(parent.currMousePos.X, parent.currMousePos.Y)) {}

			/* Functions to check event type / state. */
			bool IsValid() const noexcept { return eventType != Type::Invalid; }
			Type GetType() const noexcept { return eventType; }
			Vector2D GetPosition() const noexcept { return { mousePosition }; }// Returns mouse position.
			float GetXPosition() const noexcept { return mousePosition.X; }// Returns mouses X position.
			float GetYPosition() const noexcept { return mousePosition.Y; }// Returns mouses Y position.
			bool IsLeftDown() const noexcept { return leftDown; }// Is the left mouse button down.
			bool IsRightDown() const noexcept { return rightDown; }// Is the right mouse button down.
			bool IsMiddleDown() const noexcept { return middleDown; }// Is the middle mouse button down.
		};

	public:

		/* Constructors. */
		WindowsInput() = default;
		WindowsInput(const WindowsInput&) = delete;
		WindowsInput& operator = (const WindowsInput&) = delete;

		/* MOUSE FUNCTIONALITY. */

		/* Enum to declare specific mouse buttons for input events. */
		enum class EMouseButton
		{
			Left,
			Right,
			Middle
		};

		Vector2D GetMousePosition() const noexcept;// Returns the mouse position.
		float GetMouseXPosition() const noexcept;// Returns the mouse x position.
		float GetMouseYPosition() const noexcept;// Returns the mouse y position.
		bool IsMouseDown(EMouseButton button) const noexcept;// Returns if the specified mouse button is pressed.
		bool IsMouseMoving() noexcept;// Returns if the mouse is moving.
		float GetMouseDelta() noexcept;// Returns the current mouse delta val.
		bool IsMouseInWindow() const noexcept; // Is the mouse within the owning window for this input.
		std::string GetMousePositionString() noexcept;// Return the current mouse position as a formated string.
		WindowsInput::MouseEvent ReadMouseEvent() noexcept;// Return the first event on the buffer if there is one.
		bool IsMouseBufferEmpty() const noexcept;// Return if the mouse button buffer is empty.
		void ResetMouseBuffer() noexcept;// Reset the buffer.

	private:

		void OnMouseMove(int x, int y) noexcept;// Ran when the mouse is moved within a window.
		void OnMouseInside() noexcept;// Called when mouse leaves window.
		void OnMouseOutside() noexcept;// Called when mouse enters the window.
		void OnMousePressed(EMouseButton buttonPressed) noexcept;// Ran when a mouse button is pressed.
		void OnMouseReleased(EMouseButton buttonReleased) noexcept;// Ran when a mouse button is released.
		void OnMouseWheelUp() noexcept;// Ran when mouse scroll wheel is moved up.
		void OnMouseWheelDown() noexcept;// Ran when mouse scroll wheel is moved down.
		void OnMouseWheelDelta(int delta) noexcept;// Ran when mouse scroll wheel is moved.
		void TrimMouseBuffer() noexcept;// Cuts the mouse buffer down to the mouseBufferSize starting with oldest elements.

	private:

		/* Mouse event variables to hold current states. */
		Vector2D currMousePos = Vector2D(0.0f, 0.0f);
		float wheelDelta = 0.0f;
		bool leftMouseDown, rightMouseDown, middleMouseDown = false;
		bool mouseInsideWindow = false;
		std::queue<MouseEvent> mouseBuffer;

	public:

		/* KEYBOARD FUNCTIONALITY. */

		bool IsKeyDown(unsigned char keyCode) const noexcept; // Return if key is pressed.
		KeyboardEvent ReadKeyEvent()  noexcept; // Read current key event from buffer.
		bool IsKeyEmpty() const noexcept; // Check if the key buffer is empty.
		void ResetKeyBuffer() noexcept; // Reset the key buffer to its default state.

		char ReadChar() noexcept; // Return most recent char in charBuffer.
		bool IsCharEmpty() const noexcept; // Is the char buffer empty?
		void ResetCharBuffer() noexcept; // Reset the charBuffer to its default state.
		void ResetBuffers() noexcept; // Reset both the keyBuffer and charBuffer to their default state.

		/* Autorepeat control. NOTE: If enabled will allow keyboard pressed function to run repeatedly until released. */
		void EnableAutorepeat() noexcept;
		void DisableAutorepeat() noexcept;
		bool IsAutorepeatEnabled() const noexcept;

	private:

		/* Key and char pressed/released event functions. */
		void OnKeyPressed(unsigned char keyCode, int keyCount) noexcept;
		void OnKeyReleased(unsigned char keyCode) noexcept;
		void OnCharPressed(char character) noexcept;
		void ResetKeyState() noexcept; // Reset the key set.

		/* Trims down the given buffer in terms of the oldest elements to the bufferSize variable. */
		template<typename T>
		static void TrimKeyboardBuffer(std::queue<T>& buffer) noexcept;

	private:

		/* Keyboard variables to convert key names to events etc. */
		static constexpr unsigned int keyNum = 256u;
		static constexpr unsigned int bufferSize = 16u;
		bool autorepeatEnabled = true;
		std::bitset<keyNum> keyState;
		std::queue<KeyboardEvent> keyBuffer;
		std::queue<char> charBuffer;
	};
}



