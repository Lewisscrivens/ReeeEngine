#pragma once
#include "../Renderable.h"

namespace ReeeEngine
{
	/* Extension of a renderable object to draw a box shaped object onto the D3D render texture with a position, rotation and scale. */
	class Box : public Renderable<Box>
	{
	public:

		/* Init function for setting transform of the box. 
		 * NOTE: Where rotation vector x = roll, y = pitch and z = yaw */
		Box(Graphics& graphics, Vector3D location = Vector3D(0.0f, 0.0f, 0.0f), Rotator rotation = Rotator(0.0f, 0.0f, 0.0f), Vector3D scale = Vector3D(1.0f, 1.0f, 1.0f));

		/* Tick override for updating the box per engine frame. */
		virtual void Tick(float deltaTime) noexcept override;
	};
}
