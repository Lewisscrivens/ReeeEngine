#pragma once
#include "../Globals.h"
#include "../ReeeLog.h"
#include "../Math/Vector2D.h"
#include "opencv2/tracking.hpp"
#include "opencv2/opencv.hpp"
#include <thread>

/* Helper macros from throwing opencv log messages from booleans. */
#define OPENCV_WARNING_CHECK(result, ...) if(!result) { REEE_LOG(Warning, "OpenCVInput Warning: ", __VA_ARGS__); }
#define OPENCV_ERROR_CHECK_RETURN(result, ...) if(!result) { REEE_LOG(Error, "OpenCVInput Error: ", __VA_ARGS__); return false; }

namespace ReeeEngine
{
	/* Tracking types to choose from. */
	enum class TrackType
	{
		BOOSTING,
		MIL,
		KCF, 
		TLD,
		MEDIANFLOW,
		GOTURN,
		MOSSE,
		CSRT
	};

	/* Wrapper class for the use of OpenCV tracking features as input to drive game mechanics. */
	class OpenCVInput
	{
	public:

		/* Use default constructor/destructor. Not needed in my use case. */
		OpenCVInput(TrackType typeOfTracking);
		~OpenCVInput();

		/* Re-initalise tracking. */
		void ReinitTracking(float delay = 0.0f);

		/* Update the currentFrame and tracking for opencv input. */
		bool Update();

		/* Get the the height difference alpha between the two boxes y-diff. 
		 * Use this as input to drive game mechanics for steering something. */
		float GetHeightDiffernce() const;

		/* Get the current tracking offset of either the right or left tracker. */
		Vector2D GetLeftTrackingOffset() const;
		Vector2D GetRightTrackingOffset() const;

		/* Has open-cv be intialised and ready for update? */
		bool IsInitialised() const { return initialisedComponents; }

	private:

		/* Initalise tracking type. */
		cv::Ptr<cv::Tracker> InitialiseTracker(TrackType type);

		/* Initalise the video capture device. */
		bool InitialiseCaptureDevice();

		/* Reset tracking state of trackers. */
		void ResetTracking();

		/* Destroys the threads that handle tracking... */
		void DestroyTrackerThreads();

		/* Start updating the tracking for left and right side in their own thread. */
		void StartTrackingUpdate();

	private:

		/* Tracking information for a tracker. */
		struct TrackerInformation
		{
			cv::Ptr<cv::Tracker> tracker;    // Current tracking algorithm in use from opencv::tracking.
			TrackType trackType;			 // Current tracking type.
			cv::Rect2d trackedPosition;      // The current tracking position.
			cv::Rect2d originalPosition;     // The original tracking position.
		};

		std::thread* leftThread = nullptr;   // Left hand side tracking update thread.
		std::thread* rightThread = nullptr;  // Right hand side tracking update thread.
		TrackerInformation leftTracker;		 // Left hand side tracker.
		TrackerInformation rightTracker;	 // Right hand side tracker.
		TrackType currentTrackType;			 // The current type of tracking.

		std::thread* imageCapThread;		 // Image capturing thread for writing to my current frame outside of the application update loop.
		bool initialisedComponents = false;  // Components for OpenCV have been successfully created.

		class Timer* timer = nullptr;		 // Timer for reseting opencv input with delay to get into position.
		float currentResetDelay = 0.0f;		 // Time to reset tracking at.
		bool initialisedTrackers = false;    // Has intialised the current tracker?	
		bool runningTrackers = false;		 // Should the trackers be running?

		cv::VideoCapture currentCamera;		 // Current camera device capturing and writing to the currentFrame cv::mat.
		cv::Mat currentFrame;				 // Latest frame updated from the video capture device.
	};
}