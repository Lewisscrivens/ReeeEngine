#include "Graphics.h"
#include "DXErrors/dxerr.h"
#include "DxgiMessageManager.h"
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

namespace ReeeEngine
{
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

		// Create depth stencil.
		D3D11_DEPTH_STENCIL_DESC depthStencilOptions = {};
		depthStencilOptions.DepthEnable = TRUE;
		depthStencilOptions.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilOptions.DepthFunc = D3D11_COMPARISON_LESS;
		WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		GRAPHICS_THROW_INFO(device->CreateDepthStencilState(&depthStencilOptions, &depthStencilState));

		// Bind depth stencil state to the context.
		context->OMSetDepthStencilState(depthStencilState.Get(), 1u);

		// Create depth stencil texture.
		WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
		D3D11_TEXTURE2D_DESC depthStencilTextureOptions = {};
		depthStencilTextureOptions.Width = 800u;
		depthStencilTextureOptions.Height = 600u;
		depthStencilTextureOptions.MipLevels = 1u;
		depthStencilTextureOptions.ArraySize = 1u;
		depthStencilTextureOptions.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilTextureOptions.SampleDesc.Count = 1u;
		depthStencilTextureOptions.SampleDesc.Quality = 0u;
		depthStencilTextureOptions.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTextureOptions.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		GRAPHICS_THROW_INFO(device->CreateTexture2D(&depthStencilTextureOptions, nullptr, &depthStencilTexture));

		// Create view of depth stencil texture.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewOptions = {};
		depthStencilViewOptions.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewOptions.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewOptions.Texture2D.MipSlice = 0u;
		GRAPHICS_THROW_INFO(device->CreateDepthStencilView(depthStencilTexture.Get(), &depthStencilViewOptions, &depthStencil));

		// Bind stencil view to the render target binded to the window class....
		context->OMSetRenderTargets(1u, renderTarget.GetAddressOf(), depthStencil.Get());
	}

	void Graphics::EndFrame()
	{
		// If in debug mode set graphics message manager to clear last frames messages.
#ifndef DEBUG_ENABLED
		messageManager.ClearMessages();
#endif

		// Target 60 fps with no extra flags. Also handle if device is removed / driver crash with correct error code.
		HRESULT result = swapChain->Present(1u, 0u);
		if (FAILED(result))
		{
			if (result == DXGI_ERROR_DEVICE_REMOVED) throw GRAPHICS_LOST_EXCEPT(device->GetDeviceRemovedReason());
			else throw GRAPHICS_EXCEPT_INFO(result);
		}
	}

	void Graphics::ClearRenderBuffer(float r, float g, float b) noexcept
	{
		// Clears the render target view 
		const float color[] = { r, g, b, 1.0f };
		context->ClearRenderTargetView(renderTarget.Get(), color);
		context->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void Graphics::DrawCube(float angle, float x, float y)
	{
		HRESULT hr;

		// Vertex structure.
		struct Vertex
		{
			struct
			{
				float x;
				float y;
				float z;
			} pos;
		};

		// Create the cube in a vertex array.
		Vertex cube[] =
		{
			{ -1.0f,-1.0f,-1.0f	 },
			{ 1.0f,-1.0f,-1.0f	 },
			{ -1.0f,1.0f,-1.0f	 },
			{ 1.0f,1.0f,-1.0f	  },
			{ -1.0f,-1.0f,1.0f	 },
			{ 1.0f,-1.0f,1.0f	  },
			{ -1.0f,1.0f,1.0f	 },
			{ 1.0f,1.0f,1.0f	 },
		};

		// Create the vertex buffer.
		WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(cube);
		bd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = cube;
		GRAPHICS_THROW_INFO(device->CreateBuffer(&bd, &sd, &vertexBuffer));

		// Bind vertex buffer to pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

		// Create index buffers structure.
		const unsigned short indices[] =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};

		// Create the index buffer.
		WRL::ComPtr<ID3D11Buffer> indexBuffer;
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = sizeof(indices);
		ibd.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices;
		GRAPHICS_THROW_INFO(device->CreateBuffer(&ibd, &isd, &indexBuffer));

		// Bind index buffer
		context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

		// Create constant buffer for transformation matrix
		struct ConstantBuffer
		{
			DX::XMMATRIX transform;
		};
		const ConstantBuffer cb =
		{
			{
				DX::XMMatrixTranspose(DX::XMMatrixRotationZ(angle) *DX::XMMatrixRotationX(angle) *
					DX::XMMatrixTranslation(x,y,4.0f) * DX::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f))
			}
		};

		// Create constant buffer.
		WRL::ComPtr<ID3D11Buffer> constantBuffer;
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(cb);
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &cb;
		GRAPHICS_THROW_INFO(device->CreateBuffer(&cbd, &csd, &constantBuffer));

		// Bind constant buffer to vertex shader.
		context->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());

		// Lookup table for cube face colors.
		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};

		// Create the color of the cube.
		const ConstantBuffer2 cubeColor =
		{
			{
				// R    G    B
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
			}
		};
		WRL::ComPtr<ID3D11Buffer> constantBuffer2;
		D3D11_BUFFER_DESC cbd2;
		cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd2.Usage = D3D11_USAGE_DEFAULT;
		cbd2.CPUAccessFlags = 0u;
		cbd2.MiscFlags = 0u;
		cbd2.ByteWidth = sizeof(cubeColor);
		cbd2.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd2 = {};
		csd2.pSysMem = &cubeColor;
		GRAPHICS_THROW_INFO(device->CreateBuffer(&cbd2, &csd2, &constantBuffer2));

		// bind constant buffer to pixel shader
		context->PSSetConstantBuffers(0u, 1u, constantBuffer2.GetAddressOf());

		// create pixel shader
		WRL::ComPtr<ID3D11PixelShader> pixelShader;
		WRL::ComPtr<ID3DBlob> blob;
		GRAPHICS_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &blob));
		GRAPHICS_THROW_INFO(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));

		// bind pixel shader
		context->PSSetShader(pixelShader.Get(), nullptr, 0u);

		// create vertex shader
		WRL::ComPtr<ID3D11VertexShader> vertexShader;
		GRAPHICS_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &blob));
		GRAPHICS_THROW_INFO(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));

		// bind vertex shader
		context->VSSetShader(vertexShader.Get(), nullptr, 0u);

		// input (vertex) layout (2d position only)
		WRL::ComPtr<ID3D11InputLayout> inputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		GRAPHICS_THROW_INFO(device->CreateInputLayout(
			ied, (UINT)std::size(ied),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&inputLayout
		));

		// Bind vertex layout
		context->IASetInputLayout(inputLayout.Get());

		// Bind render target
		context->OMSetRenderTargets(1u, renderTarget.GetAddressOf(), nullptr);

		// Set primitive topology to triangle list (groups of 3 vertices)
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1u, &vp);
		GRAPHICS_THROW_INFO_ONLY(context->DrawIndexed((UINT)std::size(indices), 0u, 0u));
	}
}


