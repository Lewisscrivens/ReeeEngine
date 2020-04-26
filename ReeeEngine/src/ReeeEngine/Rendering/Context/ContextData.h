#pragma once
#include "../Graphics.h"

namespace ReeeEngine
{
	/* Class to implement graphics binding from the context for context objects like the vertex shader, vertex buffer etc. */
	class  ContextData
	{
	public:

		/* Use default destructor and create virtual add function for children classes to bind to a context object. */
		virtual void Add(Graphics& graphics) noexcept = 0;
		virtual ~ContextData() = default;

	protected:

		/* Getters and setters for returning components of graphics to its children. */
		static ID3D11DeviceContext* GetContext(Graphics& graphics) noexcept;
		static ID3D11Device* GetDevice(Graphics& graphics) noexcept;
	};
}