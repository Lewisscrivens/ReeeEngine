#pragma once

/* Platform definitions. */
#ifdef PLATFORM_WINDOWS
	#ifdef REEE_BUILD
		#define REEE_API __declspec(dllexport)
	#else
		#define REEE_API __declspec(dllimport)
	#endif
#else 
	#error Windows only!!!
#endif

/* Engine debugging macros. */
#define DEBUG_ENABLED 1;