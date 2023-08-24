#pragma once
using EventResult = RE::BSEventNotifyControl;

class ModCallbackEventHandler : public RE::BSTEventSink<SKSE::ModCallbackEvent>
{
public:
	virtual EventResult ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource);

	static bool Register();
};
