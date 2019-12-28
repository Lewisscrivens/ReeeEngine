#include "Input.h"
#include "Window.h"
#include <sstream>

std::pair<int, int> Input::GetMousePosition() const noexcept
{
	return {mouseX, mouseY};
}

int Input::GetMouseXPosition() const noexcept
{
	return mouseX;
}

int Input::GetMouseYPosition() const noexcept
{
	return mouseY;
}

bool Input::IsMouseDown(EMouseButton button) const noexcept
{
	// Return if the mouse button is currently down.
	switch (button)
	{
	case Input::EMouseButton::Left:
		return leftMouseDown;
	break;
	case Input::EMouseButton::Right:
		return rightMouseDown;
	break;
	case Input::EMouseButton::Middle:
		return middleMouseDown;
	break;
	}

	// Just in case it makes it past.
	return false;
}

bool Input::IsMouseMoving() noexcept
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

int Input::GetMouseDelta() noexcept
{
	return wheelDelta;
}

bool Input::IsMouseInWindow() const noexcept
{
	return mouseInsideWindow;
}

std::string Input::GetMousePositionString() noexcept
{
	std::ostringstream mouseStringStream;
	mouseStringStream << "Mouse Position: (" << mouseX << ", " << mouseY;
	return mouseStringStream.str();
}

Input::MouseEvent Input::ReadMouseEvent() noexcept
{
	// If there are any events stored in the mouse buffer read front event.
	if (mouseBuffer.size() > 0u)
	{
		Input::MouseEvent newMouseEvent = mouseBuffer.front(); // Use newest event.
		mouseBuffer.pop(); // Remove oldest event.
		return newMouseEvent;
	}
	else return Input::MouseEvent(); // Otherwise return default empty event.
}

bool Input::IsMouseBufferEmpty() const noexcept
{
	return mouseBuffer.empty();
}

void Input::ResetMouseBuffer() noexcept
{
	mouseBuffer = std::queue<MouseEvent>();
}

void Input::OnMouseMove(int x, int y) noexcept
{
	// Update mouse x and y.
	mouseX = x;
	mouseY = y;

	// Add new event to mouse buffer and remove oldest event.
	mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::Move, *this));
	TrimMouseBuffer();
}

void Input::OnMouseInside() noexcept
{
	// Update mouse inside window event.
	mouseInsideWindow = true;
	mouseBuffer.push(MouseEvent(MouseEvent::Type::InsideWindow, *this));
	TrimMouseBuffer();
}

void Input::OnMouseOutside() noexcept
{
	// Update mouse inside window event.
	mouseInsideWindow = false;
	mouseBuffer.push(MouseEvent(MouseEvent::Type::OutsideWindow, *this));
	TrimMouseBuffer();
}

void Input::OnMousePressed(EMouseButton buttonPressed) noexcept
{
	// Add event to mouse buffer.
	switch (buttonPressed)
	{
	case Input::EMouseButton::Left:
		leftMouseDown = true;
		mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::LeftDown, *this));
	break;
	case Input::EMouseButton::Right:
		rightMouseDown = true;
		mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::RightDown, *this));
	break;
	case Input::EMouseButton::Middle:
		middleMouseDown = true;
		mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::MiddleDown, *this));
	break;
	}

	// Remove oldest event from mouse buffer.
	TrimMouseBuffer();
}

void Input::OnMouseReleased(EMouseButton buttonReleased) noexcept
{
	// Add event to mouse buffer.
	switch (buttonReleased)
	{
	case Input::EMouseButton::Left:
		leftMouseDown = false;
		mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::LeftUp, *this));
		break;
	case Input::EMouseButton::Right:
		rightMouseDown = false;
		mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::RightUp, *this));
		break;
	case Input::EMouseButton::Middle:
		middleMouseDown = false;
		mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::MiddleUp, *this));
		break;
	}

	// Remove oldest event from mouse buffer.
	TrimMouseBuffer();
}

void Input::OnMouseWheelUp() noexcept
{
	mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::ScrollUp, *this));

	// Remove oldest event from mouse buffer.
	TrimMouseBuffer();
}

void Input::OnMouseWheelDown() noexcept
{
	mouseBuffer.push(Input::MouseEvent(MouseEvent::Type::ScrollDown, *this));

	// Remove oldest event from mouse buffer.
	TrimMouseBuffer();
}

void Input::OnMouseWheelDelta(int delta) noexcept
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

void Input::TrimMouseBuffer() noexcept
{
	// Remove oldest mouse event from buffer until its within the desired buffer size.
	while (mouseBuffer.size() > bufferSize)
	{
		mouseBuffer.pop();
	}
}

bool Input::IsKeyDown(unsigned char keyCode) const noexcept
{
	return keySet[keyCode];
}

Input::KeyboardEvent Input::ReadKeyEvent() noexcept
{
	// If there is any keys in the buffer read and return event.
	if (keyBuffer.size() > 0u)
	{
		Input::KeyboardEvent event = keyBuffer.front();
		keyBuffer.pop();// Remove oldest element.
		return event;
	}
	// Otherwise return an empty event which is default set to invalid.
	else return Input::KeyboardEvent();
}

bool Input::IsKeyEmpty() const noexcept
{
	return keyBuffer.empty();
}

void Input::ResetKeyBuffer() noexcept
{
	keyBuffer = std::queue<KeyboardEvent>();
}

char Input::ReadChar() noexcept
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

bool Input::IsCharEmpty() const noexcept
{
	return charBuffer.empty();
}

void Input::ResetCharBuffer() noexcept
{
	charBuffer = std::queue<char>();
}

void Input::ResetBuffers() noexcept
{
	ResetKeyBuffer();
	ResetCharBuffer();
}

void Input::EnableAutorepeat() noexcept
{
	autorepeatEnabled = true;
}

void Input::DisableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool Input::IsAutorepeatEnabled() const noexcept
{
	return autorepeatEnabled;
}

void Input::OnKeyPressed(unsigned char keyCode) noexcept
{
	// Set key pressed to true in keySet and add the event to the key buffer.
	keySet[keyCode] = true;
	keyBuffer.push(Input::KeyboardEvent{ Input::KeyboardEvent::Type::Down, keyCode });

	// Trim of the oldest key event in the key buffer.
	TrimKeyboardBuffer(keyBuffer);
}

void Input::OnKeyReleased(unsigned char keyCode) noexcept
{
	// Set key pressed to false in keySet and add the event to the key buffer.
	keySet[keyCode] = false;
	keyBuffer.push(Input::KeyboardEvent{ Input::KeyboardEvent::Type::Up, keyCode });

	// Trim off oldest key event in the key buffer.
	TrimKeyboardBuffer(keyBuffer);
}

void Input::OnCharPressed(char character) noexcept
{
	// Add character pressed to charBuffer.
	charBuffer.push(character);

	// Trim off oldest char in buffer.
	TrimKeyboardBuffer(charBuffer);
}

void Input::ClearKeySet() noexcept
{
	keySet.reset();
}

template<typename T>
void Input::TrimKeyboardBuffer(std::queue<T>& buffer) noexcept
{
	// When the buffer becomes too big, trim down to the correct buffer size.
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}