#pragma once
#include "Delegate.h"
#include "../Globals.h"

namespace ReeeEngine
{
	/* Delegate called when a window is resized. */
	class REEE_API WindowResizedDelegate : public Delegate
	{
	public:

		// Initalise what the window has been resized to.
		WindowResizedDelegate(unsigned int newWidth, unsigned int newHeight);

		// Return the new width and height of the resized window.
		int GetNewWidth() const;
		int GetNewHeight() const;

		// Return a description of the delegate/event.
		std::string ToString() override;

		// Return the delegate type.
		virtual DelegateType GetType() const override { return DelegateType::Window; };

	private:

		// The new width and height of this delegate when called.
		unsigned int width;
		unsigned int height;
	};

	/* Delegate called when a window is closed. */
	class REEE_API WindowClosedDelegate : public Delegate
	{
	public:

		// Default constructor. Not currently in use.
		WindowClosedDelegate() = default;

		// Return a description of the delegate/event.
		std::string ToString() override;

		// Return the delegate type.
		virtual DelegateType GetType() const override { return DelegateType::Window; };
	};
}
