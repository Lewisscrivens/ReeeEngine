#include "Sphere.h"
#include "Shapes.h"
#include "../../../Globals.h"
#include "../../Context/ContextIncludes.h"

namespace ReeeEngine
{
	Sphere::Sphere(Graphics& graphics, float sphereRadius, Vector3D location , Vector3D rotation , Vector3D scale)
	{
		// Set starting world location/rotation/scale.
		SetLocation(location);
		SetRotation(rotation);
		SetScale(scale);

		// Is Intialised.
		if (!IsInitialised())
		{
			// Bind vertex shader.
			auto vertexShaderReff = CreateReff<VertexShader>(graphics, L"../bin/Debug-x64/ReeeEngine/VertexShader.cso");
			auto vertexShaderBytecode = vertexShaderReff->GetBytecode();
			AddStaticData(std::move(vertexShaderReff));

			// Bind pixel shader.
			AddStaticData(CreateReff<PixelShader>(graphics, L"../bin/Debug-x64/ReeeEngine/PixelShader.cso"));
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
			
			// Setup input layout and topology.
			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputSettings =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticData(CreateReff<InputLayout>(graphics, inputSettings, vertexShaderBytecode));
			AddStaticData(CreateReff<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		// Create the sphere mesh and bind it to the pipeline.
		auto newSphere = SphereShape::MakeSphere<Vertex>(sphereRadius);
		AddData(CreateReff<VertextData>(graphics, newSphere.vertices));
		AddIndexData(CreateReff<IndexData>(graphics, newSphere.indices));
		AddData(CreateReff<TransformData>(graphics, *this));
	}

	void Sphere::Tick(float deltaTime) noexcept
	{
		//...
	}
}
