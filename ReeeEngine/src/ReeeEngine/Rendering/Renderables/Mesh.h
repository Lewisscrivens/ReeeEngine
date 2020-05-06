#pragma once
#include "Renderable.h"

namespace ReeeEngine
{
	/* A triangulated mesh that is loaded using the assimp loading library. */
	class Mesh : public Renderable<Mesh>
	{
	public:

		/* Mesh constructor from a given file. */
		Mesh(Graphics& graphics, const std::string& filePath, Vector3D location, Rotator rotation, 
			Vector3D scale, float importScale = 1.0f, bool lit = false);
	};
}

