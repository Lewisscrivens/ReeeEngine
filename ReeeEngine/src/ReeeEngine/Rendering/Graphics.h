#pragma once
#include "../Globals.h"
#include "../ReeeLog.h"
#include "../Math/ReeeMath.h"
#include "../Math/Vector2D.h"
#include "DXErrors/dxerr.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <wrl.h>
#include <memory>
#include <random>

/* Macros for throwing errors when DirectX functions are not working as intended. */
#define LOG_DX_ERROR(hr) if(FAILED(hr)) { REEE_LOG(Error, "DirectX Error: {0}: {1} (Line: {2}, File: {3})", DXGetErrorString(hr), DXError::GetDescription(hr), __LINE__, __FILE__); DXError::ThrowErrorBox(hr, __LINE__, __FILE__);  __debugbreak(); }

namespace ReeeEngine
{
	/* Define the default projection settings. */
	struct ProjectionSettings
	{
		float fov = 60.0f;
		float width = 1280.0f;
		float height = 720.0f;
		float nearClip = 0.5f;
		float farClip = 2000.0f;
	};

	/* Define quick class for getting error descriptions from the dxgi interface. */
	static class REEE_API DXError
	{
	public:
		static std::string GetDescription(HRESULT result)
		{
			char buf[512];
			DXGetErrorDescription(result, buf, sizeof(buf));
			return buf;
		}

		static void ThrowErrorBox(HRESULT result, int lineNo, std::string fileName)
		{
			// Create the exception string for the window using a string steam.
			std::ostringstream exceptionString;
			exceptionString << "DirectX Graphics Exception: " << std::endl
				<< "[Error Code] " << result << std::endl
				<< "[Error String] " << DXGetErrorString(result) << std::endl
				<< "[Description] " << GetDescription(result) << std::endl
				<< "[LineNo] " << lineNo << std::endl
				<< "[File] " << fileName;		

			// Open message box.
			MessageBox(nullptr, exceptionString.str().c_str(), "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
		}
	};

	/* Create and handle graphics device. */
	class REEE_API Graphics
	{
		/* Allow to access private variables. */
		friend class ContextData;

	public:

		/* Constructor. Initialize and create the device, swap chain and context objects.
		 * NOTE: Disable copying and moving of a graphics class as we do not need that functionality. */
		Graphics(HWND hWnd, int width, int height);
		Graphics(const Graphics&) = delete;
		Graphics& operator = (const Graphics&) = delete;
		~Graphics() = default;

		/* Function for resizing the render targets when the window size is changed.
		 * NOTE: No input will simply re-initalise the current width and height. */
		void ResizeRenderTargets(int width = 0, int height = 0);

		/* End frame function. */
		void EndFrame();

		/* Clears the render buffer state to a given color.
		 * NOTE: By default with no input it is cleared to black. */
		void ClearRenderBuffer(float r = 0.0f, float g = 0.0f, float b = 0.0f) noexcept;

		/* Draw any context data binded to the rendering pipeline. */
		void Draw(UINT numberOfIndex);

		/* Set the current rendering projection matrix functions. */
		void SetProjectionSettings(float fov = 0.0f, float newWidth = 0.0f, float newHeight = 0.0f, float nearClip = 0.0f, float farClip = 0.0f) noexcept;
		void SetProjectionMatrix(DirectX::FXMMATRIX projectionMatrix) noexcept;

		/* Return the current projection matrix. */
		DirectX::XMMATRIX GetProjectionMatrix() const noexcept;

		/* Device getter. */
		ID3D11Device* GetDevice() { return device.Get(); }

		/* Context getter. */
		ID3D11DeviceContext* GetContext() { return context.Get(); }

	private:

		/* Saved projection matrix settings. */
		ProjectionSettings projectionSettings;

		/* Save viewport size. */
		Vector2D viewportSize;

		/* Create graphics device variables. */
		/* NOTE: ComPtr handles releasing after application shutdown making destructor's unnecessary. */
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencil;

		/* Current projection matrix. */
		DirectX::XMMATRIX projectionMatrix;
	};
}