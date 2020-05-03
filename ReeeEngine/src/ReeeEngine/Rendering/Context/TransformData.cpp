#include "TransformData.h"
#include "../../Application.h"
#include "../../World/World.h"
#include "../../World/Components/CameraComponent.h"

namespace ReeeEngine
{
	std::unique_ptr<VertexConstantBuffer<MeshTransform>> TransformData::transformData;
	TransformData::TransformData(Graphics& graphics, const RenderableMesh& parent) : parent(parent)
	{
		// If the transform data has not been intialised initalise the pointer.
		if (!transformData)
		{
			transformData = std::make_unique<VertexConstantBuffer<MeshTransform>>(graphics);
		}
	}

	void TransformData::Add(Graphics& graphics) noexcept
	{
		// Generate model view and project matrix for constant buffer.
		const DirectX::XMMATRIX modelViewMatrix = parent.GetTransform() * Application::GetApp().GetWorld()->GetActiveCamera().GetViewMatrix();
		const MeshTransform modelTransform =
		{
			DirectX::XMMatrixTranspose(modelViewMatrix),
			DirectX::XMMatrixTranspose(modelViewMatrix * Application::GetApp().GetWorld()->GetActiveCamera().GetProjectionMatrix())
		};

		// Update the models transform and add it to the graphics pipeline.
		transformData->Update(graphics, modelTransform);
		transformData->Add(graphics);
	}
}
