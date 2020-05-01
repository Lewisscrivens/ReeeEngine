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
		RenderableMesh() = default;
		RenderableMesh(const RenderableMesh&) = delete;
		virtual ~RenderableMesh() = default;

		/* Get the renderable transform position. */
		virtual DirectX::XMMATRIX GetTransform() const noexcept;

		/* Render the position of the renderable to the render texture on the pipeline. */
		void Render(Graphics& graphics) const noexcept;

		/* Tick the renderable and update per frame. */
		virtual void Tick(float deltaTime) noexcept = 0;	

		/* Set location after init. */
		void SetLocation(Vector3D newRotation);

		/* Set rotation after init.
		 * NOTE: In degrees. GetTransform will convert it to radians. */
		void SetRotation(Rotator newRotation);

		/* Function to adjust mesh scale after initializations. */
		void SetScale(Vector3D newScale);

		/* Get location in the world. */
		Vector3D GetLocation();

		/* Get rotation in the world. */
		Rotator GetRotation();

		/* Get scale in the world. */
		Vector3D GetScale();

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

		// Position of the mesh in the world.
		Vector3D worldLocation = Vector3D(0.0f, 0.0f, 0.0f);

		// Rotation of the mesh around its own origin.
		// NOTE: In degrees. Use ToRadians function to convert to radian rotator.
		Rotator worldRotation = Rotator(0.0f, 0.0f, 0.0f);

		// Scale of the mesh within the world.
		Vector3D worldScale = Vector3D(0.0f, 0.0f, 0.0f);
	};
}