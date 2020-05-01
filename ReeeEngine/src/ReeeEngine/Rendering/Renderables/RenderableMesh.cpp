#include "RenderableMesh.h"
#include "../Context/IndexData.h"
#include <cassert>
#include <typeinfo>

namespace ReeeEngine
{
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
	
	DirectX::XMMATRIX RenderableMesh::GetTransform() const noexcept
	{
		Rotator rotRad = worldRotation.ToRadians();
		return DirectX::XMMatrixScaling(worldScale.X, worldScale.Y, worldScale.Z) * DirectX::XMMatrixRotationRollPitchYaw(rotRad.Pitch, rotRad.Yaw, rotRad.Roll) *
			DirectX::XMMatrixTranslation(worldLocation.X, worldLocation.Y, worldLocation.X) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}

	void RenderableMesh::SetLocation(Vector3D newLocation)
	{
		worldLocation = newLocation;
	}

	void RenderableMesh::SetRotation(Rotator newRotation)
	{
		worldRotation = newRotation;
	}

	void RenderableMesh::SetScale(Vector3D newScale)
	{
		worldScale = newScale;
	}

	Vector3D RenderableMesh::GetLocation()
	{
		return worldLocation;
	}

	Rotator RenderableMesh::GetRotation()
	{
		return worldRotation;
	}

	Vector3D RenderableMesh::GetScale()
	{
		return worldScale;
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