#include "OpenCVInput.h"
#include "../Application.h"

namespace ReeeEngine
{
	OpenCVInput::OpenCVInput(TrackType typeOfTracking)
	{
		// Initalise opencv input objects.
		bool result = false;
		result = InitialiseCaptureDevice();
		CHECK_RETURN(result);
		currentTrackType = typeOfTracking;
		
		// Has open cv been intialised correctly.
		initialisedComponents = true;
		REEE_LOG(Log, "OpenCVInput: Initialised OpenCVInput succesfully.");
	}

	OpenCVInput::~OpenCVInput()
	{
		// Destroy instanced objects.
		if (imageCapThread)
		{
			imageCapThread->join();
			delete imageCapThread;
		}
		DestroyTrackerThreads();
	}

	void OpenCVInput::DestroyTrackerThreads()
	{
		runningTrackers = false;
		if (leftThread)
		{
			leftThread->join();
			delete leftThread;
			leftThread = nullptr;
		}
		if (rightThread)
		{
			rightThread->join();
			delete rightThread;
			rightThread = nullptr;
		}
	}

	cv::Ptr<cv::Tracker> OpenCVInput::InitialiseTracker(TrackType type)
	{
		// Initalise the correct type of tracker.
		switch (type)
		{
		case TrackType::BOOSTING:
			return cv::Ptr<cv::TrackerBoosting>(cv::TrackerBoosting::create());
		    break;
		case TrackType::MIL:
			return cv::Ptr<cv::TrackerMIL>(cv::TrackerMIL::create());
		    break;
		case TrackType::KCF:	
			return cv::Ptr<cv::TrackerKCF>(cv::TrackerKCF::create());
		    break;
		case TrackType::TLD:
			return cv::Ptr<cv::TrackerTLD>(cv::TrackerTLD::create());
		    break;
		case TrackType::MEDIANFLOW:	
			return cv::Ptr<cv::TrackerMedianFlow>(cv::TrackerMedianFlow::create());
			break;
		case TrackType::GOTURN:
			return cv::Ptr<cv::TrackerGOTURN>(cv::TrackerGOTURN::create());
			break;
		case TrackType::MOSSE:
			return cv::Ptr<cv::TrackerMOSSE>(cv::TrackerMOSSE::create());
			break;
		case TrackType::CSRT:
			return cv::Ptr<cv::TrackerCSRT>(cv::TrackerCSRT::create());
			break;
		}

		// Perform log checks.
		REEE_LOG(Log, "Tracker wasnt created... Returning null.");
		return nullptr;
	}

	bool OpenCVInput::InitialiseCaptureDevice()
	{
		// Open selected camera using selected default API
		currentCamera.open(0, cv::CAP_ANY);

		// Check if the capture device was opened correctly.
		OPENCV_ERROR_CHECK_RETURN(currentCamera.isOpened(), "Camera was not initialised!!! Returning");
		REEE_LOG(Log, "OpenCVInput: Initialised video capture device.");

		// Run image capturing on separate thread.
		imageCapThread = new std::thread([this]() mutable
		{
			while (Application::GetEngine().IsAppRunning())
			{
				// Read the current camera frame into our opencv::mat
				currentCamera.read(currentFrame);
				cv::cvtColor(currentFrame, currentFrameBW, cv::COLOR_BGR2GRAY);
			}
		});
	}

	void OpenCVInput::StartTrackingUpdate()
	{
		// Start left thread.
		leftThread = new std::thread([this]() mutable
		{
			while (runningTrackers)
			{
				// Run update algorithm...
				leftTracker.tracker->update(currentFrameBW, leftTracker.trackedPosition);
			}
		});

		// Start right thread.
		rightThread = new std::thread([this]() mutable
		{
			while (runningTrackers)
			{
				// Run update algorithm...
				rightTracker.tracker->update(currentFrameBW, rightTracker.trackedPosition);
			}
		});

		// Set trackers to running...
		runningTrackers = true;		
	}

	void OpenCVInput::ReinitTracking(float delay)
	{
		// Setup tracking delay...
		if (delay != 0.0f)
		{
			timer = new Timer();
			currentResetDelay = delay;
		}
		else
		{
			ResetTracking();
			currentResetDelay = 0.0f;
		}

		// Reset tracking threads.
		DestroyTrackerThreads();
	}

	void OpenCVInput::ResetTracking()
	{
		// Reset the tracker if created otherwise just create a new one.
		initialisedTrackers = false;
		if (leftTracker.tracker.get()) leftTracker.tracker.release();
		if (rightTracker.tracker.get()) rightTracker.tracker.release();
		leftTracker.tracker = InitialiseTracker(currentTrackType);
		rightTracker.tracker = InitialiseTracker(currentTrackType);
		if (leftTracker.tracker.get() == nullptr || rightTracker.tracker.get() == nullptr)
		{
			REEE_LOG(Warning, "OpenCVInput Error: Failed to initialise trackers.");
			return;
		}

		// Intialised the left tracker.
		leftTracker.trackedPosition.x = 140;
		leftTracker.trackedPosition.y = 200;
		leftTracker.trackedPosition.width = 80;
		leftTracker.trackedPosition.height = 80;
		leftTracker.originalPosition = leftTracker.trackedPosition;

		// Intialised the right tracker.
		rightTracker.trackedPosition.x = 400;
		rightTracker.trackedPosition.y = 200;
		rightTracker.trackedPosition.width = 80;
		rightTracker.trackedPosition.height = 80;
		rightTracker.originalPosition = rightTracker.trackedPosition;

		// Init trackers and return warning message if anything fails.
		bool leftResult = leftTracker.tracker->init(currentFrameBW, leftTracker.trackedPosition);
		bool rightResult = rightTracker.tracker->init(currentFrameBW, rightTracker.trackedPosition);
		if (leftResult && rightResult) initialisedTrackers = true;
		else REEE_LOG(Warning, "Failed to reset tracking system for opencv.");
	}

	bool OpenCVInput::Update()
	{
		// Check if we succeeded reading the frame from the camera.
		if (currentFrame.empty())
		{
			REEE_LOG(Warning, "Open cv empty frame detected returning out of update function...");
			return false;
		}

		// If first update reset trackers to start tracking in 5 seconds.
		if (!initialisedTrackers)
		{
			ResetTracking();
			ReinitTracking(5.0f);
		}

		// If waiting for reset, reset when ready.
		if (currentResetDelay > 0.0f && timer)
		{
			bool readyForReset = timer->GetTime() >= currentResetDelay;
			if (readyForReset)
			{
				ResetTracking();
				currentResetDelay = 0.0f;
				if (timer)
				{
					delete timer;
					timer = nullptr;
				}
			}
			else
			{
				float flashSpeed = 0.2f;
				bool colorChange = (((int)(timer->GetTime() / flashSpeed) % 2) == 0);
				cv::Scalar trackColor = cv::Scalar(0, 255, 0);
				if (colorChange) trackColor = cv::Scalar(0, 0, 255);
				cv::rectangle(currentFrame, leftTracker.originalPosition, trackColor);
				cv::rectangle(currentFrame, rightTracker.originalPosition, trackColor);
			}
		}
		else
		{
			// Update the tracking...
			if (!runningTrackers)
			{
				StartTrackingUpdate();
			}

			// Draw the overlays to show where tracking is taking place.
			cv::rectangle(currentFrame, leftTracker.trackedPosition, cv::Scalar(0, 255, 0));
			cv::rectangle(currentFrame, rightTracker.trackedPosition, cv::Scalar(0, 255, 0));
		}

		// Show image in new window.
		cv::Mat flipped;
		cv::flip(currentFrame, flipped, +1);
		imshow("OpenCVInput", flipped);
	}

	float OpenCVInput::GetHeightDiffernce() const
	{
		Vector2D direction = (Vector2D((float)leftTracker.trackedPosition.x, (float)leftTracker.trackedPosition.y) - 
			Vector2D((float)rightTracker.trackedPosition.x, (float)rightTracker.trackedPosition.y)).GetNormal();
		return direction.Y;
	}

	Vector2D OpenCVInput::GetLeftTrackingOffset() const
	{
		return (Vector2D((float)leftTracker.trackedPosition.x, (float)leftTracker.trackedPosition.y) -
			Vector2D((float)leftTracker.originalPosition.x, (float)leftTracker.originalPosition.y));
	}

	Vector2D OpenCVInput::GetRightTrackingOffset() const
	{
		return (Vector2D((float)rightTracker.trackedPosition.x, (float)rightTracker.trackedPosition.y) -
			Vector2D((float)rightTracker.originalPosition.x, (float)rightTracker.originalPosition.y));
	}
}