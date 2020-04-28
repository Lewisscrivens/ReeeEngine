#pragma once
#include "../Globals.h"

// Macro for binding functions to delegates.
#define BIND_DELEGATE(func) std::bind(&func, this, std::placeholders::_1)

namespace ReeeEngine
{
	/* Declare main application event types. */
	enum class DelegateType
	{
		None = 0,
		Input,
		Window,
		Application,
	};

	/* Delegate base class for binding events to other functions. */
	class REEE_API Delegate
	{
		friend class DelegateDispatcher;

	public:

		// Return the delegate type 
		// NOTE: Needs to be overridden and set in each new delegate class/type.
		virtual DelegateType GetType() const = 0;

		// Return description of the delegate.
		virtual std::string ToString() = 0;

		// Has the delegate been handled by the dispatcher?
		bool handled = false;
	};

	/* Dispatcher class for dispatching new delegate events. */
	class DelegateDispatcher
	{
		// Use std::function variable as Function.
		template<typename T>
		using Function = std::function<bool(T&)>;

	public:

		// Default constructor for a new delegate dispatcher.
		DelegateDispatcher(Delegate& newDelegate) : currDelegate(newDelegate) {};

		// Dispatch a new binded delegate and mark as handled.
		template<typename T>
		bool Dispatch(Function<T> function)
		{
			// NOTE: NEEDS PROFILING TO ENSURE THIS ISNT A BIG MISTAKE....
			// Ensure that the current delegate is the correct type dispatched.
			if (!dynamic_cast<T*>(&currDelegate)) return false;

			// Call and handle the binded delegate and mark as handled.
			currDelegate.handled = function(static_cast<T&>(currDelegate));
			return true;
		}

	private:

		// Current delegate being handled.
		Delegate& currDelegate;
	};
}