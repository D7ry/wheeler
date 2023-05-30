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
	/// Process the input event, and decide whether to deny the event dispatch to the rest of the game.
	/// </summary>
	/// <param name="a_event">ptr to a list of input events</param>
	/// <returns>True if the event will be dispatched to the game, false otherwise.</returns>
	bool ProcessAndDeny(RE::InputEvent* const* a_event);
};
