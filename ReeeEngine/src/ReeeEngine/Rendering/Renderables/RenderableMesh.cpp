#include "RenderableMesh.h"
#include "../Context/IndexData.h"
#include <cassert>
#include <typeinfo>

namespace ReeeEngine
{
	RenderableMesh::RenderableMesh()
	{
		// Set default transform to 0 rotation/rotation in all axis with 1 scale in all axis.
		meshTransform = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	}

	void RenderableMesh::Render(Graphics& graphics) const noexcept
	{
		// Bind the context data to the graphics pipeline.
		for (auto& data : pContextData)
		{
			data->Add(graphics);
		}
		for (auto& data : GetStaticData())
		{
			data->Add(graphics);
		}

		// Draw the index array of this renderable mesh.
		graphics.Draw(pIndexData->GetNum());
	}

	void RenderableMesh::SetTransform(const DirectX::XMMATRIX& newTransform)
	{
		meshTransform = newTransform;
	}

	DirectX::XMMATRIX RenderableMesh::GetTransform() const noexcept
	{
		return meshTransform;
	}

	void RenderableMesh::AddData(Refference<ContextData> data) noexcept
	{
		assert("Have to use AddIndexData to bind index data to the pipeline!!!" && typeid(*data) != typeid(IndexData));
		pContextData.push_back(std::move(data));
	}

	void RenderableMesh::AddIndexData(Refference<IndexData> iData) noexcept
	{
		assert("Attempting to add index data a second time" && pIndexData == nullptr);
		pIndexData = iData.get();
		pContextData.push_back(std::move(iData));
	}
}