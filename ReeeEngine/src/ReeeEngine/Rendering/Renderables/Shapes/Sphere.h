#pragma once
#include "../Renderable.h"

namespace ReeeEngine
{
	/* Extension of a renderable object to draw a box shaped object onto the D3D render texture with a position, rotation and scale. */
	class Sphere : public Renderable<Sphere>
	{
	public:

		/* Init function for setting up a new solid sphere mesh. */
		Sphere(Graphics& graphics, float sphereRadius = 1.0f, Vector3D location = Vector3D(0.0f, 0.0f, 0.0f), 
			Rotator rotation = Rotator(0.0f, 0.0f, 0.0f), Vector3D scale = Vector3D(1.0f, 1.0f, 1.0f));
	};
}
