#include "Box.h"
#include "Shapes.h"
#include "../../../Globals.h"
#include "../../Context/ContextIncludes.h"

namespace ReeeEngine
{
	Box::Box(Graphics& graphics, Vector3D location, Vector3D rotation, Vector3D scale)
	{
		// Set starting world location/rotation/scale.
		SetLocation(location);
		SetRotation(rotation);
		SetScale(scale);

		// If not yet intialised, initalise the D3D Context with this boxes context data.
		if (!IsInitialised())
		{
			// Create triangle list of a cube shape.
			const auto newCubeShape = CubeShape::NewCube<Vertex>();
			AddStaticData(CreateReff<VertextData>(graphics, newCubeShape.vertices));

			// Create vertex shader to use.
			auto vertexShaderReff = CreateReff<VertexShader>(graphics, L"../bin/Debug-x64/ReeeEngine/VertexShader.cso");
			auto vertexShaderBytecode = vertexShaderReff->GetBytecode();
			AddStaticData(std::move(vertexShaderReff));

			// Create pixel shader to use.
			AddStaticData(CreateReff<PixelShader>(graphics, L"../bin/Debug-x64/ReeeEngine/PixelShader.cso"));
			AddStaticIndexData(CreateReff<IndexData>(graphics, newCubeShape.indices));
			struct PixelShaderConstants
			{
				struct
				{
					float r;
					float g;
					float b;
					float a;
				} face_colors[8];
			};
			const PixelShaderConstants cb2 =
			{
				{
					{ 1.0f,1.0f,1.0f },
					{ 1.0f,0.0f,0.0f },
					{ 0.0f,1.0f,0.0f },
					{ 1.0f,1.0f,0.0f },
					{ 0.0f,0.0f,1.0f },
					{ 1.0f,0.0f,1.0f },
					{ 0.0f,1.0f,1.0f },
					{ 0.0f,0.0f,0.0f },
				}
			};
			AddStaticData(CreateReff<PixelConstantBuffer<PixelShaderConstants>>(graphics, cb2));

			// Create topology and input layout.
			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticData(CreateReff<InputLayout>(graphics, ied, vertexShaderBytecode));
			AddStaticData(CreateReff<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else SetStaticIndexData();

		// Add transform data to the context.
		AddData(CreateReff<TransformData>(graphics, *this));
	}

	void Box::Tick(float deltaTime) noexcept
	{
		//...
	}
}