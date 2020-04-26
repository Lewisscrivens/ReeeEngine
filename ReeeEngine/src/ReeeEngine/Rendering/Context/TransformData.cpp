#include "TransformData.h"

namespace ReeeEngine
{
	std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformData::transformData;
	TransformData::TransformData(Graphics& graphics, const RenderableMesh& parent) : parent(parent)
	{
		// If the transform data has not been intialised initalise the pointer.
		if (!transformData)
		{
			transformData = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(graphics);
		}
	}

	void TransformData::Add(Graphics& graphics) noexcept
	{
		transformData->Update(graphics, DirectX::XMMatrixTranspose(parent.GetTransform() * graphics.GetProjectionMatrix()));
		transformData->Add(graphics);
	}
}
