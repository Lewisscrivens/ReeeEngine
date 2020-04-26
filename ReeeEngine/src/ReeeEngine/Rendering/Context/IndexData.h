#pragma once
#include "ContextData.h"

namespace ReeeEngine
{ 
	/* Input data class to act as a wrapper for creating new IndexBuffers and binding them to the device. */
	class IndexData : public ContextData
	{
	public:

		/* Default constructor for creating new index data. */
		IndexData(Graphics& graphics, const std::vector<unsigned short>& indexArray);
		
		/* Override the bind function of the context data parent class. */
		virtual void Add(Graphics& graphics) noexcept override;

		/* Get number of index's in index array. */
		UINT GetNum() const noexcept;

	protected:

		UINT numberOfIndex;// The number of indexes in current index data class.
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexData;// The index buffer/data.
	};
}