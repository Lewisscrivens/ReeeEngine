#include "IndexData.h"

namespace ReeeEngine
{
	IndexData::IndexData(Graphics& graphics, const std::vector<unsigned short>& indexArray)
	{
		// Setup number of indeces.
		numberOfIndex = ((UINT)indexArray.size());

		// Create new index buffer to bind to the graphics device.
		D3D11_BUFFER_DESC newIndexData = {};
		newIndexData.BindFlags = D3D11_BIND_INDEX_BUFFER;
		newIndexData.Usage = D3D11_USAGE_DEFAULT;
		newIndexData.CPUAccessFlags = 0u;
		newIndexData.MiscFlags = 0u;
		newIndexData.ByteWidth = UINT(numberOfIndex * sizeof(unsigned short));
		newIndexData.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA newIndexResources = {};
		newIndexResources.pSysMem = indexArray.data();

		// Attempt to bind the new index data to the graphics device, throw errors if any are returned.
		LOG_DX_ERROR(GetDevice(graphics)->CreateBuffer(&newIndexData, &newIndexResources, &indexData));
	}

	void IndexData::Add(Graphics& graphics) noexcept
	{
		GetContext(graphics)->IASetIndexBuffer(indexData.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	UINT IndexData::GetNum() const noexcept
	{
		return numberOfIndex;
	}
}