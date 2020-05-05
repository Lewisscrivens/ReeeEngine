#pragma once
#include <vector>
#include "../../../Math/ReeeMath.h"

namespace ReeeEngine
{
	/* List of indeces base class for other shapes. */
	template<class T>
	class MeshBase
	{
	public:

		/* Default constructors for creating a new triangle list. */
		MeshBase() = default;
		MeshBase(std::vector<T> vertexList, std::vector<unsigned short> indexList) : vertices(std::move(vertexList)), indices(std::move(indexList))
		{
			assert(vertices.size() > 2);
			assert(indices.size() % 3 == 0);
		}

		/* Transform the vertices's and indices relative to the transform matrix passed in. */
		void Transform(DirectX::FXMMATRIX transformMatrix)
		{
			// Loop through each vertex and transform it relative to its current position and the passed transform matrix.
			for (auto& vertex : vertices)
			{
				const DirectX::XMVECTOR vertexPos = DirectX::XMLoadFloat3(&vertex.pos);
				DirectX::XMStoreFloat3(&vertex.pos, DirectX::XMVector3Transform(vertexPos, transformMatrix));
			}
		}

		/* Set up face-independent vertices w/ normals cleared to zero... 
		 * NOTE: Planet chilli function... */
		void SetNormalsIndependentFlat()
		{
			assert(indices.size() % 3 == 0 && indices.size() > 0);
			for (size_t i = 0; i < indices.size(); i += 3)
			{
				auto& v0 = vertices[indices[i]];
				auto& v1 = vertices[indices[i + 1]];
				auto& v2 = vertices[indices[i + 2]];
				const auto p0 = DirectX::XMLoadFloat3(&v0.pos);
				const auto p1 = DirectX::XMLoadFloat3(&v1.pos);
				const auto p2 = DirectX::XMLoadFloat3(&v2.pos);
				const auto n = DirectX::XMVector3Normalize(DirectX::XMVector3Cross((p1 - p0), (p2 - p0)));
				DirectX::XMStoreFloat3(&v0.n, n);
				DirectX::XMStoreFloat3(&v1.n, n);
				DirectX::XMStoreFloat3(&v2.n, n);
			}
		}

	public:

		// The indeces and vertices within this triangulated mesh object.
		std::vector<T> vertices;
		std::vector<unsigned short> indices;
	};

	/* Cube shape vertex/indeces list. */
	class CubeShape
	{
	public:

		template<class V>
		static MeshBase<V> NewCube()
		{
			// Create each vertex of a cube and add it to the verteces list.
			constexpr float side = 1.0f / 2.0f;
			std::vector<DirectX::XMFLOAT3> vertices;
			vertices.emplace_back(-side, -side, -side);
			vertices.emplace_back(side, -side, -side);
			vertices.emplace_back(-side, side, -side);
			vertices.emplace_back(side, side, -side);
			vertices.emplace_back(-side, -side, side);
			vertices.emplace_back(side, -side, side);
			vertices.emplace_back(-side, side, side);
			vertices.emplace_back(side, side, side);

			// Position verteces in a box shape.
			std::vector<V> verts(vertices.size());
			for (size_t i = 0; i < vertices.size(); i++)
			{
				verts[i].pos = vertices[i];
			}

			// Return verts and indeces to present a cube mesh.
			return{std::move(verts),
			{   0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4 } 
			};
		}
	};

	/* Plane shape vertex/indeces list. */
	class PlaneShape
	{
	public:

		/* Function to create a plane with any given x and y divisions.
		 * NOTE: Default is 1 division in the x and y with no input into the constructor... */
		template<class V>
		static MeshBase<V> MakePlane(int xDiv = 1, int yDiv = 1)
		{
			// Make sure that the number of x and y divisions are above 1.
			assert(xDiv >= 1);
			assert(yDiv >= 1);

			// Calculate the number of vertices and default spacing.
			constexpr float width = 2.0f;
			constexpr float height = 2.0f;
			const int vertNumX = xDiv + 1;
			const int vertNumY = yDiv + 1;
			std::vector<V> vertices(vertNumX * vertNumY);

			// Calculate number of verteces relative to the division amount.
			const float sideX = width / 2.0f;
			const float sideY = height / 2.0f;
			const float divisionSizeX = width / float(xDiv);
			const float divisionSizeY = height / float(yDiv);
			const auto bottomLeft = DirectX::XMVectorSet(-sideX, -sideY, 0.0f, 0.0f);

			// For each vertex along the x and y-axis add it to the set of vectors in the correct position.
			for (int y = 0, i = 0; y < vertNumY; y++)
			{
				const float yPosition = float(y) * divisionSizeY;
				for (int x = 0; x < vertNumX; x++, i++)
				{
					// Create new vert and add to list.
					DirectX::XMVECTOR newVert = DirectX::XMVectorAdd(bottomLeft, DirectX::XMVectorSet(float(x) * divisionSizeX, yPosition, 0.0f, 0.0f));
					DirectX::XMStoreFloat3(&vertices[i].pos, newVert);
				}
			}

			// Create indices for each vertex.
			std::vector<unsigned short> indices;
			indices.reserve(ReeeMath::Squared(xDiv * yDiv) * 6);
			
			// Lambda define for returning an index position for each division.
			const auto GetIndexPosition = [vertNumX](size_t x, size_t y)
			{
				return (unsigned short)(y * vertNumX + x);
			};

			// For each devision add index in the order to create a plane mesh.
			for (size_t y = 0; y < yDiv; y++)
			{
				for (size_t x = 0; x < xDiv; x++)
				{
					const std::array<unsigned short, 4> indexArray = { GetIndexPosition(x, y), GetIndexPosition(x + 1, y), GetIndexPosition(x, y + 1), GetIndexPosition(x + 1, y + 1) };
					indices.push_back(indexArray[0]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[3]);
				}
			}

			// Return verts and indeces to present a plane mesh with the specified number of x and y divisions.
			return{std::move(vertices), std::move(indices)};
		}
	};

	/* Sphere shape vertex/indeces list. */
	class SphereShape
	{
	public:

		/* Function to create a sphere mesh with any given horizontal and vertical divisions.
		 * NOTE: Default is 12 divisions horizontally and 24 divisions vertically with no constructor input... */
		template<class V>
		static MeshBase<V> MakeSphere(float radius = 1.0f, int horizonalDiv = 12, int verticalDiv = 24)
		{
			// Ensure that the horizontal and vertical divisions are both above 3 as its the minimum required to make a sphere.
			assert(horizonalDiv >= 3);
			assert(verticalDiv >= 3);

			// Create transform for creating sections of the sphere with the correct number of divisions.
			const auto transform = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
			const float horizontalAngle = PI / horizonalDiv;
			const float verticalAngle = 2.0f * PI / verticalDiv;

			// Create vertices of the sphere mesh.
			std::vector<V> vertices;
			for (int h = 1; h < horizonalDiv; h++)
			{
				DirectX::XMVECTOR horizontalPos = DirectX::XMVector3Transform(transform, DirectX::XMMatrixRotationX(horizontalAngle * h));
				for (int v = 0; v < verticalDiv; v++)
				{
					vertices.emplace_back();
					DirectX::XMVECTOR verticalPos = DirectX::XMVector3Transform(horizontalPos, DirectX::XMMatrixRotationZ(verticalAngle * v));
					DirectX::XMStoreFloat3(&vertices.back().pos, verticalPos);
				}
			}

			// Cap the top and bottom of the mesh.
			const auto topPole = (unsigned short)vertices.size();
			vertices.emplace_back();
			DirectX::XMStoreFloat3(&vertices.back().pos, transform);
			const auto bottomPole = (unsigned short)vertices.size();
			vertices.emplace_back();
			DirectX::XMStoreFloat3(&vertices.back().pos, DirectX::XMVectorNegate(transform));

			// Lambda function for returning index position of the current horizontal and vertical division.
			const auto GetIndexPosition = [horizonalDiv, verticalDiv](unsigned short h, unsigned short v)
			{ 
				return h * verticalDiv + v; 
			};

			// Create indeces list for the sphere mesh with the correct number of divisions.
			std::vector<unsigned short> indices;
			for (unsigned short h = 0; h < horizonalDiv - 2; h++)
			{
				for (unsigned short v = 0; v < verticalDiv - 1; v++)
				{
					indices.push_back(GetIndexPosition(h, v));
					indices.push_back(GetIndexPosition(h + 1, v));
					indices.push_back(GetIndexPosition(h, v + 1));
					indices.push_back(GetIndexPosition(h, v + 1));
					indices.push_back(GetIndexPosition(h + 1, v));
					indices.push_back(GetIndexPosition(h + 1, v + 1));
				}
				
				// Wrap.
				indices.push_back(GetIndexPosition(h, verticalDiv - 1));
				indices.push_back(GetIndexPosition(h + 1, verticalDiv - 1));
				indices.push_back(GetIndexPosition(h, 0));
				indices.push_back(GetIndexPosition(h, 0));
				indices.push_back(GetIndexPosition(h + 1, verticalDiv - 1));
				indices.push_back(GetIndexPosition(h + 1, 0));
			}

			// Cap top and bottom.
			for (unsigned short v = 0; v < verticalDiv - 1; v++)
			{
				// Top
				indices.push_back(topPole);
				indices.push_back(GetIndexPosition(0, v));
				indices.push_back(GetIndexPosition(0, v + 1));
				
				// Bottom
				indices.push_back(GetIndexPosition(horizonalDiv - 2, v + 1));
				indices.push_back(GetIndexPosition(horizonalDiv - 2, v));
				indices.push_back(bottomPole);
			}

			// Position indeces for top and bottom cap.
			indices.push_back(topPole);
			indices.push_back(GetIndexPosition(0, verticalDiv - 1));
			indices.push_back(GetIndexPosition(0, 0));
			indices.push_back(GetIndexPosition(horizonalDiv - 2, 0));
			indices.push_back(GetIndexPosition(horizonalDiv - 2, verticalDiv - 1));
			indices.push_back(bottomPole);

			// Return verts and indeces to present a sphere mesh with the specified number of horizontal and vertical divisions with the given radius.
			return {std::move(vertices), std::move(indices)};
		}
	};
}