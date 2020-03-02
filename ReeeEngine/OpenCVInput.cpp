#include "OpenCVInput.h"

namespace ReeeEngine
{
	OpenCVInput::OpenCVInput()
	{

	}

	void OpenCVInput::InitialiseCamera()
	{
		pWebcam = &cv::VideoCapture(0);
		if (!pWebcam.Get()->isOpened())
		{

		}
	}

}