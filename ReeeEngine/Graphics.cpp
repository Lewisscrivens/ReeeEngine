#include "Graphics.h"
#include "DXErrors/dxerr.h"
#include "GraphicsException.h"
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


