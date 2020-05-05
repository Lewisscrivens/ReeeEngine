#include "Sphere.h"
#include "Shapes.h"
#include "../../../Globals.h"
#include "../../Context/ContextIncludes.h"

namespace ReeeEngine
{
	Sphere::Sphere(Graphics& graphics, float sphereRadius, Vector3D location , Rotator rotation , Vector3D scale)
	{
		// Set starting world location/rotation/scale.
		SetLocation(location);
		SetRotation(rotation);
		SetScale(scale);

		// Is Intialised.
		if (!IsInitialised())
		{
			// Create the sphere mesh and bind it to the pipeline.
			auto newSphere = SphereShape::MakeSphere<Vertex>(sphereRadius);
			AddData(CreateReff<VertextData>(graphics, newSphere.vertices));
			AddIndexData(CreateReff<IndexData>(graphics, newSphere.indices));

			// Bind vertex shader.
			auto vertexShaderReff = CreateReff<VertexShader>(graphics, L"../bin/Debug-x64/ReeeEngine/LitColorVS.cso");
			auto vertexShaderBytecode = vertexShaderReff->GetBytecode();
			AddStaticData(std::move(vertexShaderReff));

			// Bind pixel shader.
			AddStaticData(CreateReff<PixelShader>(graphics, L"../bin/Debug-x64/ReeeEngine/LitColorPS.cso"));
			
			// Bind lit color for LitColorPS to use when rendering.
			struct PSLitColor
			{
				DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
				float padding;
			} sphereColor;
			AddStaticData(CreateReff<PixelConstantBuffer<PSLitColor>>(graphics, sphereColor));

			// Setup input layout and topology.
			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputSettings =
			{
				{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			AddStaticData(CreateReff<InputLayout>(graphics, inputSettings, vertexShaderBytecode));
			AddStaticData(CreateReff<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else SetStaticIndexData();

		// Setup transform data.
		AddData(CreateReff<TransformData>(graphics, *this));
	}
}
