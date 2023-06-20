#pragma once
#include "PCH.h"
#include <unordered_set>

class Input
{
public:
	static Input* GetSingleton()
	{
		static Input listener;
		return std::addressof(listener);
	}

	/// <summary>
	/// Process the input event, and filter out unwanted input events through pointer adjustments.
	/// </summary>
	/// <param name="a_event">ptr to a list of input events</param>
	void ProcessAndFilter(RE::InputEvent** a_event);

	static inline const std::unordered_set<std::string> EventsToFilterWhenWheelerActive = {
		"Favorites",
		"Inventory",
		"Stats",
		"Map",
		"Tween Menu",
		"Quick Inventory",
		"Quick Magic",
		"Quick Stats",
		"Quick Map",
		"Wait",
		"Journal"
	};

};
