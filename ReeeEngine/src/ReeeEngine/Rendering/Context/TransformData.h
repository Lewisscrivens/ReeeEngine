#pragma once
#include "ConstantBuffer.h"
#include "../Renderables/RenderableMesh.h"
#include <DirectXMath.h>

namespace ReeeEngine
{
	/* Model transform constant value for shaders. */
	struct MeshTransform
	{
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX modelViewProj;
	};

	/* Transform data for any context data being passed into the rendering pipeline. */
	class TransformData : public ContextData
	{
	public:

		/* Transform data constructor. */
		TransformData(Graphics& graphics, const RenderableMesh& parent);

		/* Transform data binding to the pipeline/context object. */
		virtual void Add(Graphics& graphics) noexcept override;

	private:

		static std::unique_ptr<VertexConstantBuffer<MeshTransform>> transformData;
		const RenderableMesh& parent;
	};
}


