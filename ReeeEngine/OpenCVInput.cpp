#include "OpenCVInput.h"

namespace ReeeEngine
{
	OpenCVInput::OpenCVInput()
	{

	}

	void OpenCVInput::InitialiseCamera()
	{
		pWebcam = std::make_shared<cv::VideoCapture>(cv::VideoCapture(0));
		if (!pWebcam->isOpened())
		{

		}
	}

}