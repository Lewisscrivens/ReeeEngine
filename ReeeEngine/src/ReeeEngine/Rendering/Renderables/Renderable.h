#pragma once
#include "RenderableMesh.h"
#include "../Context/IndexData.h"

namespace ReeeEngine
{
	/* Template class for creating a renderable object that is added to the rendering pipeline and drawn 
	 * using data from classes such as the topology, transform, vertex and constant buffers. */
	template<class T>
	class Renderable : public RenderableMesh
	{
	protected:

		/* Is the renderable mesh intialised? */
		static bool IsInitialised() noexcept
		{
			return !staticData.empty();
		}

		/* Add any form of context data to the renderable. */
		static void AddStaticData(Refference<ContextData> data) noexcept
		{
			assert("Have to use AddMeshIndexData to bind index data to the pipeline!!!" && typeid(*data) != typeid(IndexData));
			staticData.push_back(std::move(data));
		}

		/* Add index data to the current renderable. */
		void AddStaticIndexData(Refference<IndexData> iData) noexcept
		{
			assert("Attempting to add index mesh data a second time" && pIndexData == nullptr);
			pIndexData = iData.get();
			staticData.push_back(std::move(iData));
		}

		/* Setup the renderable index data to the current renderable. */
		void SetStaticIndexData() noexcept
		{
			// Find and add the index context data to the rendering pipeline.
			assert("Attempting to add index buffer a second time" && pIndexData == nullptr);
			for (const auto& data : staticData)
			{
				if (const auto dataPointer = dynamic_cast<IndexData*>(data.get()))
				{
					pIndexData = dataPointer;
					return;
				}
			}

			// If no index data was found throw error.
			assert("No index data found in the mesh data." && pIndexData != nullptr);
		}
	private:

		/* Return an array of the context data added to this renderable. */
		const std::vector<Refference<ContextData>>& GetStaticData() const noexcept override
		{
			return staticData;
		}

	private:

		// Renderable mesh data within the current renderable.
		static std::vector<Refference<ContextData>> staticData;
	};

	template<class T>
	std::vector<Refference<ContextData>> Renderable<T>::staticData;
}