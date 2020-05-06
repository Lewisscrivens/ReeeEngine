#pragma once
#include "../Graphics.h"
#include "../../Math/ReeeMath.h"
#include "../../Math/Vector3D.h"
#include "../../Math/Rotator.h"

namespace ReeeEngine
{
	// Define classes used.
	class ContextData;

	/* Renderable class to parent anything that is a loaded mesh.
	 * NOTE: Contains the functions needed to update and render a object with vertexes to the rendering texture. */
	class RenderableMesh
	{
		template<class T>
		friend class Renderable;

	public:

		/* Use default constructors/destructors. */
		RenderableMesh();
		RenderableMesh(const RenderableMesh&) = delete;
		virtual ~RenderableMesh() = default;

		/* Get/Set the renderable transform position. */
		void SetTransform(const DirectX::XMMATRIX& newTransform);
		virtual DirectX::XMMATRIX GetTransform() const noexcept;

		/* Render the position of the renderable to the render texture on the pipeline. */
		void Render(Graphics& graphics) const noexcept;

	protected:

		/* Add context data to the renderable for example a constant buffer or transform... */
		void AddData(Refference<ContextData> data) noexcept;

		/* Add index data to the renderable on where its indices are placed/arranged. */
		void AddIndexData(Refference<class IndexData> iData) noexcept;

	private:

		// Return all context data binded to this Renderable.
		virtual const std::vector<Refference<ContextData>>& GetStaticData() const noexcept = 0;

	private:

		// Pointers to created index and context data like vertex arrays, index arrays, constant buffers etc.
		const class IndexData* pIndexData = nullptr;
		std::vector<Refference<ContextData>> pContextData;

		// Position of the mesh in the world for rendering purposes.
		DirectX::XMMATRIX meshTransform;
	};
}