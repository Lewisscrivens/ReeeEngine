#pragma once
#include "ReeeWin.h"
#include "ReeeException.h"
#include "Input.h"
#include <optional>
#include <memory>
#include "Graphics.h"

/* Helper window macros. */
#define WINDOW_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define WINDOW_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define WINDOW_NOGRAPHICS_EXCEPT() Window::NoGraphicsException(__LINE__, __FILE__)

/* Class to manage registration and cleanup for a given window. */
class Window
{
public:

	/* Window exception class to hold static error code translate function. */
	class Exception : public ReeeException
	{

		using ReeeException::ReeeException;

	public:

		/* Returns formated error code string from HRESULT exception. */
		static std::string TranslateErrorMessage(HRESULT hResult) noexcept;

	};

	/* Create another error exception for the window for HRESULT failures. */
	class HrException : public Exception
	{
	public:

		/* Constructor. */
		HrException(int line, const char* file, HRESULT hResult) noexcept;

		/* Exception message override. */
		const char* what() const noexcept override;

		/* Type of message override. */
		virtual const char* GetType() const noexcept override;

		/* Gets the hResult from an error code. */
		HRESULT GetErrorCode() const noexcept;

		/* Returns a string describing the error. Translates this exceptions hResult to a string. */
		std::string GetErrorInfo() const noexcept;

	private:

		/* Pointer to the hResult. */
		HRESULT hResult;
	};

	/* No graphics exception class to indicate what exception was hit. */
	class NoGraphicsException : public Exception
	{

		/* Use exception classes constructor. */
		using Exception::Exception;

	public:

		/* Return the type of exception in this case a no graphics exception. */
		const char* GetType() const noexcept override;
	};

private:

	/* Static window class to hold initialized windows variables. */
	class WindowClass
	{
	private:

		/* Constructor for registration and destructor for un-registration. */
		WindowClass() noexcept;
		~WindowClass();

		/* Handle window class operators. */
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		
		/* Create variables for this class for the name and reference to the hInstance for the window. */
		static constexpr const char* windowClassName = "Reee Engine";
		static WindowClass windowClass;
		HINSTANCE hInstance;

	public:

		/* Return the windowClassName. */
		static const char* GetName() noexcept;

		/* Return the hInstance for this window. */
		static HINSTANCE GetInstance() noexcept;

	};
	
public:

	/* Constructor and destructor. */
	Window(int width, int height, const char* name);
	~Window();

	/* Operator overrides. */
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	/* Window functions. */
	void SetTitle(const std::string& newTitle);// Set the windows title.

	/* Window message processor. */
	std::optional<int> DispatchMessages();

	/* Returns pointer to graphics. */
	Graphics& GetGraphics();

private:

	/* Message handling functions to perform actions based of user input with the window class. */
	static LRESULT CALLBACK HandleMessageEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMessageRuntime(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	/* Window variables. */
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> graphics;

public:

	/* Public input manager/receiver. */
	Input input;

	/* Return the windows hWnd. */
	HWND GetHwnd();

};

