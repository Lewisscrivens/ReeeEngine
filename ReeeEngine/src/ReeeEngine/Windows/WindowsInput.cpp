#include "WindowsInput.h"
#include "Window.h"
#include <sstream>

namespace ReeeEngine
{
	Vector2D WindowsInput::GetMousePosition() const noexcept
	{
		return currMousePos;
	}

	float WindowsInput::GetMouseXPosition() const noexcept
	{
		return currMousePos.X;
	}

	float WindowsInput::GetMouseYPosition() const noexcept
	{
		return currMousePos.Y;
	}

	bool WindowsInput::IsMouseDown(EMouseButton button) const noexcept
	{
		// Return if the mouse button is currently down.
		switch (button)
		{
		case WindowsInput::EMouseButton::Left:
			return leftMouseDown;
			break;
		case WindowsInput::EMouseButton::Right:
			return rightMouseDown;
			break;
		case WindowsInput::EMouseButton::Middle:
			return middleMouseDown;
			break;
		}

		// Just in case it makes it past.
		return false;
	}

	bool WindowsInput::IsMouseMoving() noexcept
	{
		// While the buffer is not empty.
		while (!IsMouseBufferEmpty())
		{
			MouseEvent mouseEvent = ReadMouseEvent();
			if (mouseEvent.GetType() == MouseEvent::Type::Move)
			{
				return true;
			}
		}

		// Otherwise mouse hasn't moved.
		return false;
	}

	float WindowsInput::GetMouseDelta() noexcept
	{
		return wheelDelta;
	}

	bool WindowsInput::IsMouseInWindow() const noexcept
	{
		return mouseInsideWindow;
	}

	std::string WindowsInput::GetMousePositionString() noexcept
	{
		return currMousePos.ToString();
	}

	WindowsInput::MouseEvent WindowsInput::ReadMouseEvent() noexcept
	{
		// If there are any events stored in the mouse buffer read front event.
		if (mouseBuffer.size() > 0u)
		{
			WindowsInput::MouseEvent newMouseEvent = mouseBuffer.front(); // Use newest event.
			mouseBuffer.pop(); // Remove oldest event.
			return newMouseEvent;
		}
		else return WindowsInput::MouseEvent(); // Otherwise return default empty event.
	}

	bool WindowsInput::IsMouseBufferEmpty() const noexcept
	{
		return mouseBuffer.empty();
	}

	void WindowsInput::ResetMouseBuffer() noexcept
	{
		mouseBuffer = std::queue<MouseEvent>();
	}

	void WindowsInput::OnMouseMove(int x, int y) noexcept
	{
		// Update mouse x and y.
		currMousePos = Vector2D(x, y);

		// Add new event to mouse buffer and remove oldest event.
		mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::Move, *this));
		TrimMouseBuffer();
	}

	void WindowsInput::OnMouseInside() noexcept
	{
		// Update mouse inside window event.
		mouseInsideWindow = true;
		mouseBuffer.push(MouseEvent(MouseEvent::Type::InsideWindow, *this));
		TrimMouseBuffer();
	}

	void WindowsInput::OnMouseOutside() noexcept
	{
		// Update mouse inside window event.
		mouseInsideWindow = false;
		mouseBuffer.push(MouseEvent(MouseEvent::Type::OutsideWindow, *this));
		TrimMouseBuffer();
	}

	void WindowsInput::OnMousePressed(EMouseButton buttonPressed) noexcept
	{
		// Add event to mouse buffer.
		switch (buttonPressed)
		{
		case WindowsInput::EMouseButton::Left:
			leftMouseDown = true;
			mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::LeftDown, *this));
			break;
		case WindowsInput::EMouseButton::Right:
			rightMouseDown = true;
			mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::RightDown, *this));
			break;
		case WindowsInput::EMouseButton::Middle:
			middleMouseDown = true;
			mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::MiddleDown, *this));
			break;
		}

		// Remove oldest event from mouse buffer.
		TrimMouseBuffer();
	}

	void WindowsInput::OnMouseReleased(EMouseButton buttonReleased) noexcept
	{
		// Add event to mouse buffer.
		switch (buttonReleased)
		{
		case WindowsInput::EMouseButton::Left:
			leftMouseDown = false;
			mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::LeftUp, *this));
			break;
		case WindowsInput::EMouseButton::Right:
			rightMouseDown = false;
			mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::RightUp, *this));
			break;
		case WindowsInput::EMouseButton::Middle:
			middleMouseDown = false;
			mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::MiddleUp, *this));
			break;
		}

		// Remove oldest event from mouse buffer.
		TrimMouseBuffer();
	}

	void WindowsInput::OnMouseWheelUp() noexcept
	{
		mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::ScrollUp, *this));

		// Remove oldest event from mouse buffer.
		TrimMouseBuffer();
	}

	void WindowsInput::OnMouseWheelDown() noexcept
	{
		mouseBuffer.push(WindowsInput::MouseEvent(MouseEvent::Type::ScrollDown, *this));

		// Remove oldest event from mouse buffer.
		TrimMouseBuffer();
	}

	void WindowsInput::OnMouseWheelDelta(int delta) noexcept
	{
		wheelDelta += delta;

		// Generate events for every 120 delta.
		while (wheelDelta >= WHEEL_DELTA)
		{
			wheelDelta -= WHEEL_DELTA;
			OnMouseWheelUp();
		}
		while (wheelDelta <= -WHEEL_DELTA)
		{
			wheelDelta += WHEEL_DELTA;
			OnMouseWheelDown();
		}
	}

	void WindowsInput::TrimMouseBuffer() noexcept
	{
		// Remove oldest mouse event from buffer until its within the desired buffer size.
		while (mouseBuffer.size() > bufferSize)
		{
			mouseBuffer.pop();
		}
	}

	bool WindowsInput::IsKeyDown(unsigned char keyCode) const noexcept
	{
		return keySet[keyCode];
	}

	WindowsInput::KeyboardEvent WindowsInput::ReadKeyEvent() noexcept
	{
		// If there is any keys in the buffer read and return event.
		if (keyBuffer.size() > 0u)
		{
			WindowsInput::KeyboardEvent event = keyBuffer.front();
			keyBuffer.pop();// Remove oldest element.
			return event;
		}
		// Otherwise return an empty event which is default set to invalid.
		else return WindowsInput::KeyboardEvent();
	}

	bool WindowsInput::IsKeyEmpty() const noexcept
	{
		return keyBuffer.empty();
	}

	void WindowsInput::ResetKeyBuffer() noexcept
	{
		keyBuffer = std::queue<KeyboardEvent>();
	}

	char WindowsInput::ReadChar() noexcept
	{
		// If the charBuffer has chars inside get most recent char pressed and return it.
		if (charBuffer.size() > 0u)
		{
			unsigned char charCode = charBuffer.front();
			charBuffer.pop();
			return charCode;
		}
		else return 0;
	}

	bool WindowsInput::IsCharEmpty() const noexcept
	{
		return charBuffer.empty();
	}

	void WindowsInput::ResetCharBuffer() noexcept
	{
		charBuffer = std::queue<char>();
	}

	void WindowsInput::ResetBuffers() noexcept
	{
		ResetKeyBuffer();
		ResetCharBuffer();
	}

	void WindowsInput::EnableAutorepeat() noexcept
	{
		autorepeatEnabled = true;
	}

	void WindowsInput::DisableAutorepeat() noexcept
	{
		autorepeatEnabled = false;
	}

	bool WindowsInput::IsAutorepeatEnabled() const noexcept
	{
		return autorepeatEnabled;
	}

	void WindowsInput::OnKeyPressed(unsigned char keyCode) noexcept
	{
		// Set key pressed to true in keySet and add the event to the key buffer.
		keySet[keyCode] = true;
		keyBuffer.push(WindowsInput::KeyboardEvent{ WindowsInput::KeyboardEvent::Type::Down, keyCode });

		// Trim of the oldest key event in the key buffer.
		TrimKeyboardBuffer(keyBuffer);
	}

	void WindowsInput::OnKeyReleased(unsigned char keyCode) noexcept
	{
		// Set key pressed to false in keySet and add the event to the key buffer.
		keySet[keyCode] = false;
		keyBuffer.push(WindowsInput::KeyboardEvent{ WindowsInput::KeyboardEvent::Type::Up, keyCode });

		// Trim off oldest key event in the key buffer.
		TrimKeyboardBuffer(keyBuffer);
	}

	void WindowsInput::OnCharPressed(char character) noexcept
	{
		// Add character pressed to charBuffer.
		charBuffer.push(character);

		// Trim off oldest char in buffer.
		TrimKeyboardBuffer(charBuffer);
	}

	void WindowsInput::ClearKeySet() noexcept
	{
		keySet.reset();
	}

	template<typename T>
	void WindowsInput::TrimKeyboardBuffer(std::queue<T>& buffer) noexcept
	{
		// When the buffer becomes too big, trim down to the correct buffer size.
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}
}