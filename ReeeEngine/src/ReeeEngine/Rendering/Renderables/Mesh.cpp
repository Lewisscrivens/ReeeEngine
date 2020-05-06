#include "Mesh.h"
#include "../../Globals.h"
#include "../Context/ContextIncludes.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../AssetTypes/TextureAsset.h"
#include "../Context/Texture.h"
#include "../Context/SampleState.h"

namespace ReeeEngine
{
	Mesh::Mesh(Graphics& graphics, const std::string& filePath, float importScale, bool lit)
	{
		// If not yet intialised add the index data.
		if (!IsInitialised())
		{
			AddStaticData(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		// Use assimp to read the model.
		// NOTE: Currently only setup to load root mesh and texture...
		Assimp::Importer imp;
		const auto loadedModel = imp.ReadFile(filePath + ".obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		const auto loadedMesh = loadedModel->mMeshes[0];

		// Create triangle list for the loaded model.
		std::vector<Vertex> vertices;
		vertices.reserve(loadedMesh->mNumVertices);
		for (unsigned int i = 0; i < loadedMesh->mNumVertices; i++)
		{
			vertices.push_back({
				{ loadedMesh->mVertices[i].x * importScale, loadedMesh->mVertices[i].y * importScale, loadedMesh->mVertices[i].z * importScale },
				*reinterpret_cast<DirectX::XMFLOAT3*>(&loadedMesh->mNormals[i]),
				*reinterpret_cast<DirectX::XMFLOAT2*>(&loadedMesh->mTextureCoords[0][i])
				});
		}

		// Create index list of the loaded model.
		std::vector<unsigned short> indices;
		indices.reserve(loadedMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < loadedMesh->mNumFaces; i++)
		{
			const auto& face = loadedMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		// Create and bind texture if a material exists.
		// Currently just loads parents mesh material as its the only model loaded.
		if (loadedMesh->mMaterialIndex >= 0)
		{
			using namespace std::string_literals;
			auto& material = *loadedModel->mMaterials[0];
			TextureAsset* newTextureAsset = new TextureAsset();
			bool loaded = newTextureAsset->Load(filePath + ".png");
			if (loaded)
			{
				AddData(std::make_unique<Texture>(graphics, newTextureAsset));
				AddData(std::make_unique<SampleState>(graphics));
			}
			else REEE_LOG(Warning, "Failed to load texture for model. No texure or sampler state binded to the pipeline...");
		}

		// Bind index and vertices to rendering pipeline.
		AddData(std::make_unique<VertextData>(graphics, vertices));
		AddIndexData(std::make_unique<IndexData>(graphics, indices));

		// Create vertex shader to use.
		auto pvs = lit ? std::make_unique<VertexShader>(graphics, L"../bin/Debug-x64/ReeeEngine/LitTextureVS.cso") :
			std::make_unique<VertexShader>(graphics, L"../bin/Debug-x64/ReeeEngine/PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddData(std::move(pvs));

		// Create pixel shader to use.
		AddData(lit ? std::make_unique<PixelShader>(graphics, L"../bin/Debug-x64/ReeeEngine/LitTexturePS.cso") :
			std::make_unique<PixelShader>(graphics, L"../bin/Debug-x64/ReeeEngine/PhongPS.cso"));

		// Create topology and input layout.
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Texcoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		AddData(std::make_unique<InputLayout>(graphics, ied, pvsbc));

		// Create pixel shader constant buffer.
		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} pmc;
		AddData(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(graphics, pmc, 1u));

		// Add transform data to the context.
		AddData(std::make_unique<TransformData>(graphics, *this));
	}
}