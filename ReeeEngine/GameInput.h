#pragma once

/* Define direct input version. */
#define DIRECTINPUT_VERSION 0x0800

/* Link the required libraries for direct input. */
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/* Includes. */
#include <dinput.h>
#include <vector>
#include <wrl.h>
#include "ReeeMath.h"

/* Class to receive game input from peripherals supported by DirectInput. */
class GameInput
{
public:

	/* Constructor/Destructor class defaults. */
	GameInput() = default;
	GameInput(const GameInput&) = delete;
	GameInput& operator = (const GameInput&) = delete;

	/* Ran when game is started and game input is created. */
	bool Initialize(HINSTANCE, HWND, int, int);

	/* Ran each frame of the game. */
	bool Frame();

	/* Mouse functions for returning information about the current mouse input. */
	void GetMouseLocation(int&, int&);
	int GetMouseX();
	int GetMouseY();
	int GetMouseXChange();
	int GetMouseYChange();
	void ResetMousePosition();

	/* Keyboard functions to check if a key is pressed or released etc. */
	bool IsKeyDown(unsigned char);
	void KeyPressedTrigger(unsigned char);

private:

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:

	/* Pointers to the direct input instance, the keyboard and mouse. */
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse;

	/* State variables for both the mouse and keyboard. */
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	/* Variables to keep track of mouse movement. */
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY, oldMouseX, oldMouseY;

public:

	/* Keys currently pressed array. */
	bool m_keyPressed[256];

};

