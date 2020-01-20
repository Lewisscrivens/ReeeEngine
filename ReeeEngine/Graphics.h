#pragma once
#include "ReeeWin.h"
#include "DxgiMessageManager.h"
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "ReeeException.h"

/* Create and handle graphics device. */
class Graphics
{
public:
	
	/* Constructor. Initialize and create the device, swap chain and context objects.
	 * NOTE: Disable copying and moving of a graphics class as we do not need that functionality. */
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	~Graphics() = default;

	/* End frame function. */
	void EndFrame();

	/* Clears the render target to a given color.
	 * NOTE: By default with no input it is cleared to black. */
	void ClearRenderTarget(float r = 0.0f, float g = 0.0f, float b = 0.0f) noexcept;

private:

	/* Create graphics device variables. */
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;

	/* If the debug layer is enabled, create an info manager. */
#ifndef DEBUG_ENABLED
	DxgiMessageManager graphicsMessageManager;
#endif
};