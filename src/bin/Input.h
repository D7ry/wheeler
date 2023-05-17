#pragma once
#include "PCH.h"
#include <unordered_set>

class Input : public RE::BSTEventSink<RE::InputEvent*>
{
public:
	static Input* GetSingleton()
	{
		static Input listener;
		return std::addressof(listener);
	}

	virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;
	

};
