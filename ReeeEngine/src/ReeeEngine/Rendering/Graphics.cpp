#include "Graphics.h"
#include "DXErrors/dxerr.h"
#include <sstream>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <xutility>

// Namespace shorten.
namespace WRL = Microsoft::WRL;
namespace DX = DirectX;

// Link directX libraries.
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

namespace ReeeEngine
{
	Graphics::Graphics(HWND hWnd, int width, int height)
	{
		// Save viewport size.
		viewportSize = Vector2D((float)width, (float)height);

		// Create and define swap chain options for the swap chain.
		DXGI_SWAP_CHAIN_DESC swapChainOptions = {};
		swapChainOptions.BufferDesc.Width = (UINT)width;
		swapChainOptions.BufferDesc.Height = (UINT)height;
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
		swapChainOptions.Flags = 0u;

		// Create debug flags only if debug is enabled.
		UINT debugFlags = 0u;
#ifdef DEBUG_ENABLED 
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
		LOG_DX_ERROR(result);

		// Obtain the back buffer module from the swap chain to create a render target.
		WRL::ComPtr<ID3D11Resource> backBuffer;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
		LOG_DX_ERROR(result);

		// Create render target view.
		result = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);
		LOG_DX_ERROR(result);

		// Create depth stencil.
		D3D11_DEPTH_STENCIL_DESC depthStencilOptions = {};
		depthStencilOptions.DepthEnable = TRUE;
		depthStencilOptions.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilOptions.DepthFunc = D3D11_COMPARISON_LESS;
		WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		result = device->CreateDepthStencilState(&depthStencilOptions, &depthStencilState);
		LOG_DX_ERROR(result);

		// Bind depth stencil state to the context.
		context->OMSetDepthStencilState(depthStencilState.Get(), 1u);

		// Create depth stencil texture.
		WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
		D3D11_TEXTURE2D_DESC depthStencilTextureOptions = {};
		depthStencilTextureOptions.Width = (UINT)width;
		depthStencilTextureOptions.Height = (UINT)height;
		depthStencilTextureOptions.MipLevels = 1u;
		depthStencilTextureOptions.ArraySize = 1u;
		depthStencilTextureOptions.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTextureOptions.SampleDesc.Count = 1u;
		depthStencilTextureOptions.SampleDesc.Quality = 0u;
		depthStencilTextureOptions.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTextureOptions.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		result = device->CreateTexture2D(&depthStencilTextureOptions, nullptr, &depthStencilTexture);
		LOG_DX_ERROR(result);

		// Create view of depth stencil texture.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewOptions = {};
		depthStencilViewOptions.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewOptions.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewOptions.Texture2D.MipSlice = 0u;
		result = device->CreateDepthStencilView(depthStencilTexture.Get(), &depthStencilViewOptions, &depthStencil);
		LOG_DX_ERROR(result);

		// Bind stencil view to the render target binded to the window class....
		context->OMSetRenderTargets(1u, renderTarget.GetAddressOf(), depthStencil.Get());

		// Setup the viewport
		D3D11_VIEWPORT viewport;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		context->RSSetViewports(1u, &viewport);

		// Init default projection matrix.
		projectionSettings.width = (float)width;
		projectionSettings.height = (float)height;
		SetProjectionSettings();
	}

	void Graphics::ResizeRenderTargets(int width, int height)
	{
		// Save new viewport size.
		if (width != 0 && height != 0)
		{
			viewportSize = Vector2D((float)width, (float)height);
		}

		// Prepare the render target to be overwritten.
		context->OMSetRenderTargets(0, 0, 0);
		renderTarget->Release();
		HRESULT result = swapChain->ResizeBuffers(1, (UINT)viewportSize.X, (UINT)viewportSize.Y, DXGI_FORMAT_R8G8B8A8_UNORM, 0u);
		LOG_DX_ERROR(result);

		// Re-create back buffer to create the RT.
		WRL::ComPtr<ID3D11Resource> buffer = nullptr;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &buffer);
		LOG_DX_ERROR(result);

		// Create render target view.
		result = device->CreateRenderTargetView(buffer.Get(), nullptr, renderTarget.GetAddressOf());
		LOG_DX_ERROR(result);

		// Prepare the depth stencil to be overwritten.
		depthStencil->Release();

		// Create new depth stencil texture.
		WRL::ComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;
		D3D11_TEXTURE2D_DESC depthStencilTextureOptions = {};
		depthStencilTextureOptions.Width = (UINT)viewportSize.X;
		depthStencilTextureOptions.Height = (UINT)viewportSize.Y;
		depthStencilTextureOptions.MipLevels = 1u;
		depthStencilTextureOptions.ArraySize = 1u;
		depthStencilTextureOptions.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTextureOptions.SampleDesc.Count = 1u;
		depthStencilTextureOptions.SampleDesc.Quality = 0u;
		depthStencilTextureOptions.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTextureOptions.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		result = device->CreateTexture2D(&depthStencilTextureOptions, nullptr, &depthStencilTexture);
		LOG_DX_ERROR(result);

		// Create new view of depth stencil texture.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewOptions = {};
		depthStencilViewOptions.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewOptions.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewOptions.Texture2D.MipSlice = 0u;
		result = device->CreateDepthStencilView(depthStencilTexture.Get(), &depthStencilViewOptions, depthStencil.GetAddressOf());
		LOG_DX_ERROR(result);

		// Bind stencil view to the render target binded to the window class....
		context->OMSetRenderTargets(1u, renderTarget.GetAddressOf(), depthStencil.Get());

		// Setup the viewport
		D3D11_VIEWPORT viewport;
		viewport.Width = viewportSize.X;
		viewport.Height = viewportSize.Y;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		context->RSSetViewports(1u, &viewport);

		// Setup new projection matrix values and re-initalise.
		projectionSettings.width = viewportSize.X;
		projectionSettings.height = viewportSize.Y;
		SetProjectionSettings();
	}

	void Graphics::EndFrame()
	{
		// Catch any errors when presenting the swap chain.
		HRESULT result = swapChain->Present(1u, 0u);
		LOG_DX_ERROR(result);
	}

	void Graphics::ClearRenderBuffer(float r, float g, float b) noexcept
	{
		// Clears the render target view 
		const float color[] = { r, g, b, 1.0f };
		context->ClearRenderTargetView(renderTarget.Get(), color);
		context->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}

	void Graphics::Draw(UINT numberOfIndex)
	{
		context->DrawIndexed(numberOfIndex, 0u, 0u);
	}

	void Graphics::SetProjectionSettings(float fov, float newWidth, float newHeight, float nearClip, float farClip) noexcept
	{
		// Update current projection settings if changed.
		if (fov != 0.0f) projectionSettings.fov = fov;
		if (newWidth != 0.0f) projectionSettings.width = newWidth;
		if (newHeight != 0.0f) projectionSettings.height = newHeight;
		if (nearClip != 0.0f) projectionSettings.nearClip = nearClip;
		if (farClip != 0.0f) projectionSettings.farClip = farClip;

		// Set new matrix.
		auto newMatrix = DirectX::XMMatrixPerspectiveFovLH(ReeeMath::Radians(projectionSettings.fov), ReeeMath::GetAspectRatio(projectionSettings.width, projectionSettings.height), projectionSettings.nearClip, projectionSettings.farClip);
		SetProjectionMatrix(newMatrix);
	}

	void Graphics::SetProjectionMatrix(DirectX::FXMMATRIX projectionMat) noexcept
	{
		projectionMatrix = projectionMat;
	}

	DirectX::XMMATRIX Graphics::GetProjectionMatrix() const noexcept
	{
		return projectionMatrix;
	}
}


