#include "ModCallbackEventHandler.h"
#include "Config.h"
#include "UserInput/Controls.h"
#include "Wheeler/Wheeler.h"
EventResult ModCallbackEventHandler::ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource)
{
	if (!a_event) {
		return EventResult::kContinue;
	}
	if (a_event->eventName == "dmenu_updateSettings") {
		if (a_event->strArg == "Wheeler Styles" || a_event->strArg == "Wheeler Controls") {
			Config::ReadStyleConfig();
			Config::ReadControlConfig();
			Config::OffsetSizingToViewport();
			Controls::BindAllInputsFromConfig();
		}
	} else if (a_event->eventName == "dmenu_buttonCallback") {
		if (a_event->strArg == "wheeler_reset_all_wheels") {
			Wheeler::SetupDefaultWheels();
		}
	}

	return EventResult::kContinue;
}

bool ModCallbackEventHandler::Register()
{
	static ModCallbackEventHandler singleton;

	auto eventSource = SKSE::GetModCallbackEventSource();

	if (!eventSource) {
		ERROR("EventSource not found!");
		return false;
	}
	eventSource->AddEventSink(&singleton);
	INFO("Register {}", typeid(singleton).name());
	return true;
}
