#pragma once
#include "ConstantBuffer.h"
#include "../Renderables/RenderableMesh.h"
#include <DirectXMath.h>

namespace ReeeEngine
{
	/* Transform data for any context data being passed into the rendering pipeline. */
	class TransformData : public ContextData
	{
	public:

		/* Transform data constructor. */
		TransformData(Graphics& graphics, const RenderableMesh& parent);

		/* Transform data binding to the pipeline/context object. */
		virtual void Add(Graphics& graphics) noexcept override;

	private:

		static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> transformData;
		const RenderableMesh& parent;
	};
}


