#pragma once
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2/video/tracking.hpp"
#include <memory>

namespace ReeeEngine
{
	/* Input class to receive input from the OpenCV library via camera input. */
	class OpenCVInput
	{
	public:

		/* Constructor. */
		OpenCVInput();

		/* Disable operator for copying class. */
		OpenCVInput& operator = (const OpenCVInput&) = delete;

		/* Use default constructor. */
		~OpenCVInput() = default;

	private:

		/* Pointer to a web-cam once created. */
		std::shared_ptr<class cv::VideoCapture> pWebcam;

	public:

		/* Initialize a web camera through OpenCV. */
		void InitialiseCamera();

	};
}