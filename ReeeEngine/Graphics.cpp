#include "Graphics.h"
#include "DXErrors/dxerr.h"
#include <sstream>

namespace WRL = Microsoft::WRL;// Shorten WRL namespace.
#pragma comment(lib, "d3d11.lib")// Fix linker error for D3D.

Graphics::Graphics(HWND hWnd)
{
	// Create and define swap chain options using Swap chain desc structure.
	DXGI_SWAP_CHAIN_DESC swapChainOptions = {};
	swapChainOptions.BufferDesc.Width = 0;
	swapChainOptions.BufferDesc.Height = 0;
	swapChainOptions.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainOptions.BufferDesc.RefreshRate.Numerator = 0;
	swapChainOptions.BufferDesc.RefreshRate.Denominator = 0;
	swapChainOptions.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainOptions.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainOptions.SampleDesc.Count = 1;
	swapChainOptions.SampleDesc.Quality = 0;
	swapChainOptions.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainOptions.BufferCount = 1;
	swapChainOptions.OutputWindow = hWnd;
	swapChainOptions.Windowed = TRUE;
	swapChainOptions.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainOptions.Flags = 0;

	// Create debug flags only if debug is enabled.
	UINT debugFlags = 0u;
#ifndef DEBUG_ENABLED 
	debugFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create device and swap chain and the context object.
	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		debugFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainOptions,
		&swapChain,
		&device,
		nullptr,
		&context);
	GRAPHICS_THROW_INFO(result);

	// Obtain the back buffer module from the swap chain to create a render target.
	WRL::ComPtr<ID3D11Resource> backBuffer;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
	GRAPHICS_THROW_INFO(result);

	// Create render target view.
	result = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);
	GRAPHICS_THROW_INFO(result);
}

void Graphics::EndFrame()
{
	// If in debug mode set graphics message manager to clear last frames messages.
#ifndef DEBUG_ENABLED
	graphicsMessageManager.ClearMessages();
#endif

	// Target 60 fps with no extra flags. Also handle if device is removed / driver crash with correct error code.
	HRESULT result = swapChain->Present(1u, 0u);
	if (FAILED(result))
	{
		if (result == DXGI_ERROR_DEVICE_REMOVED) throw GRAPHICS_LOST_EXCEPT(device->GetDeviceRemovedReason());
		else throw GRAPHICS_EXCEPT_INFO(result);
	}
}

void Graphics::ClearRenderTarget(float r, float g, float b) noexcept
{
	// Clears the render target view 
	const float color[] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(renderTarget.Get(), color);
}

// Initialize HResultException when created.
Graphics::HResultException::HResultException(int lineNo, const char* fileName, HRESULT hResult, std::vector<std::string> messages) noexcept : GraphicsException(lineNo, fileName), hResult(hResult)
{
	// For each message in the messages list join then together into a larger message string.
	for (auto i = 0; i < messages.size(); i++)
	{
		// For each message add to message string and make a new line if its not the last message.
		message += messages[i];
		if (i < messages.size()) message.push_back('\n');
	}
}

const char* Graphics::HResultException::what() const noexcept
{
	// Format error exception into string.
	std::ostringstream formatedError;
	formatedError << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Error Info] " << GetErrorInfo() << std::endl;

	// If there is anything in the message string add it to a Further Info section.
	if (!message.empty()) formatedError << "\n[Further Info]\n" << GetGraphicsMessage() << std::endl << std::endl;
	
	// Add origin string onto the end.
	formatedError << GetOriginString();

	// Add formated error string to the exception buffer.
	exceptionBuffer = formatedError.str();

	// Return the exception buffer.
	return exceptionBuffer.c_str();
}

const char* Graphics::HResultException::GetType() const noexcept
{
	return "Reee Graphics Class Exception.";
}

HRESULT Graphics::HResultException::GetErrorCode() const noexcept
{
	return hResult;
}

std::string Graphics::HResultException::GetErrorString() const noexcept
{
	return DXGetErrorString(hResult);
}

std::string Graphics::HResultException::GetGraphicsMessage() const noexcept
{
	return message;
}

std::string Graphics::HResultException::GetErrorInfo() const noexcept
{
	char infoChar[512];	
	DXGetErrorDescription(hResult, infoChar, sizeof(infoChar));
	return infoChar;
}

const char* Graphics::GrpahicsDeviceLostException::GetType() const noexcept
{
	return "Reeee Graphics Exception -- Device was removed or lost. DXGI_ERROR_DEVICE_REMOVED.";
}

