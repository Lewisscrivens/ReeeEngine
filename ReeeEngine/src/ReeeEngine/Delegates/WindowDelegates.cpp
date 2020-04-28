#include "WindowDelegates.h"

namespace ReeeEngine
{
	WindowResizedDelegate::WindowResizedDelegate(unsigned int newWidth, unsigned int newHeight)
	{
		width = newWidth;
		height = newHeight;
	}

	int WindowResizedDelegate::GetNewWidth() const
	{
		return width;
	}

	int WindowResizedDelegate::GetNewHeight() const
	{
		return height;
	}

	std::string WindowResizedDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Window resized to: " << width << " : " << height;
		return newString.str();
	}

	std::string WindowClosedDelegate::ToString()
	{
		std::stringstream newString;
		newString << "Window was closed.";
		return newString.str();
	}
}