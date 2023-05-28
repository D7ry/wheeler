#include "Config.h"
#define SETTINGSFILE_PATH "Data\\SKSE\\Plugins\\wheeler\\Settings.ini"

bool GetBoolValue(CSimpleIniA& ini, const char* section, const char* key) {
	return ini.GetBoolValue(section, key, false);
}
void Config::ReadConfig()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(SETTINGSFILE_PATH);

	Config::Control::Wheel::CursorRadiusPerEntry = std::stof(ini.GetValue("Control.Wheel", "CursorRadiusPerEntry", ""));
	Config::Control::Wheel::DoubleActivateDisable = ini.GetBoolValue("Control.Wheel", "DoubleActivateDisable", false);

	Config::Styling::Wheel::CursorIndicatorDist = std::stof(ini.GetValue("Styling.Wheel", "CursorIndicatorDist", ""));
	Config::Styling::Wheel::CusorIndicatorArcWidth = std::stof(ini.GetValue("Styling.Wheel", "CusorIndicatorArcWidth", ""));
	Config::Styling::Wheel::CursorIndicatorArcAngle = std::stof(ini.GetValue("Styling.Wheel", "CursorIndicatorArcAngle", ""));
	Config::Styling::Wheel::CursorIndicatorTriangleSideLength = std::stof(ini.GetValue("Styling.Wheel", "CursorIndicatorTriangleSideLength", ""));
	Config::Styling::Wheel::CursorIndicatorColor = std::stoul(ini.GetValue("Styling.Wheel", "CursorIndicatorColor", ""));
	Config::Styling::Wheel::WheelIndicatorOffsetX = std::stof(ini.GetValue("Styling.Wheel", "WheelIndicatorOffsetX", ""));
	Config::Styling::Wheel::WheelIndicatorOffsetY = std::stof(ini.GetValue("Styling.Wheel", "WheelIndicatorOffsetY", ""));
	Config::Styling::Wheel::WheelIndicatorSize = std::stof(ini.GetValue("Styling.Wheel", "WheelIndicatorSize", ""));
	Config::Styling::Wheel::WheelIndicatorSpacing = std::stof(ini.GetValue("Styling.Wheel", "WheelIndicatorSpacing", ""));
	Config::Styling::Wheel::WheelIndicatorActiveColor = std::stoul(ini.GetValue("Styling.Wheel", "WheelIndicatorActiveColor", ""));
	Config::Styling::Wheel::WheelIndicatorInactiveColor = std::stoul(ini.GetValue("Styling.Wheel", "WheelIndicatorInactiveColor", ""));
	Config::Styling::Wheel::InnerCircleRadius = std::stof(ini.GetValue("Styling.Wheel", "InnerCircleRadius", ""));
	Config::Styling::Wheel::OuterCircleRadius = std::stof(ini.GetValue("Styling.Wheel", "OuterCircleRadius", ""));
	Config::Styling::Wheel::InnerSpacing = std::stof(ini.GetValue("Styling.Wheel", "InnerSpacing", ""));

	Config::Styling::Wheel::HoveredColorBegin = std::stoul(ini.GetValue("Styling.Wheel", "HoveredColorBegin", ""));
	Config::Styling::Wheel::HoveredColorEnd = std::stoul(ini.GetValue("Styling.Wheel", "HoveredColorEnd", ""));
	Config::Styling::Wheel::UnhoveredColorBegin = std::stoul(ini.GetValue("Styling.Wheel", "UnhoveredColorBegin", ""));
	Config::Styling::Wheel::UnhoveredColorEnd = std::stoul(ini.GetValue("Styling.Wheel", "UnhoveredColorEnd", ""));
	Config::Styling::Wheel::ActiveArcColorBegin = std::stoul(ini.GetValue("Styling.Wheel", "ActiveArcColorBegin", ""));
	Config::Styling::Wheel::ActiveArcColorEnd = std::stoul(ini.GetValue("Styling.Wheel", "ActiveArcColorEnd", ""));
	Config::Styling::Wheel::InActiveArcColorBegin = std::stoul(ini.GetValue("Styling.Wheel", "InActiveArcColorBegin", ""));
	Config::Styling::Wheel::InActiveArcColorEnd = std::stoul(ini.GetValue("Styling.Wheel", "InActiveArcColorEnd", ""));
	Config::Styling::Wheel::ActiveArcWidth = std::stof(ini.GetValue("Styling.Wheel", "ActiveArcWidth", ""));
	Config::Styling::Wheel::BlurOnOpen = ini.GetBoolValue("Styling.Wheel", "BlurOnOpen", "");
	Config::Styling::Wheel::SlowTimeScale = std::stof(ini.GetValue("Styling.Wheel", "SlowTimeScale", ""));
	Config::Styling::Wheel::FadeTime = std::stof(ini.GetValue("Styling.Wheel", "FadeTime", ""));
	Config::Styling::Wheel::CenterOffsetX = std::stof(ini.GetValue("Styling.Wheel", "CenterOffsetX", ""));
	Config::Styling::Wheel::CenterOffsetY = std::stof(ini.GetValue("Styling.Wheel", "CenterOffsetY", ""));

	Config::Styling::Entry::Highlight::Text::OffsetX = std::stof(ini.GetValue("Styling.Entry.Highlight.Text", "OffsetX", ""));
	Config::Styling::Entry::Highlight::Text::OffsetY = std::stof(ini.GetValue("Styling.Entry.Highlight.Text", "OffsetY", ""));
	Config::Styling::Entry::Highlight::Text::Size = std::stoi(ini.GetValue("Styling.Entry.Highlight.Text", "Size", ""));

	Config::Styling::Item::Highlight::Texture::OffsetX = std::stof(ini.GetValue("Styling.Item.Highlight.Texture", "OffsetX", ""));
	Config::Styling::Item::Highlight::Texture::OffsetY = std::stof(ini.GetValue("Styling.Item.Highlight.Texture", "OffsetY", ""));
	Config::Styling::Item::Highlight::Texture::Scale = std::stof(ini.GetValue("Styling.Item.Highlight.Texture", "Scale", ""));
	Config::Styling::Item::Highlight::Text::OffsetX = std::stof(ini.GetValue("Styling.Item.Highlight.Text", "OffsetX", ""));
	Config::Styling::Item::Highlight::Text::OffsetY = std::stof(ini.GetValue("Styling.Item.Highlight.Text", "OffsetY", ""));
	Config::Styling::Item::Highlight::Text::Size = std::stoi(ini.GetValue("Styling.Item.Highlight.Text", "Size", ""));
	Config::Styling::Item::Highlight::Desc::OffsetX = std::stof(ini.GetValue("Styling.Item.Highlight.Desc", "OffsetX", ""));
	Config::Styling::Item::Highlight::Desc::OffsetY = std::stof(ini.GetValue("Styling.Item.Highlight.Desc", "OffsetY", ""));
	Config::Styling::Item::Highlight::Desc::Size = std::stoi(ini.GetValue("Styling.Item.Highlight.Desc", "Size", ""));

	Config::Styling::Item::Slot::Texture::OffsetX = std::stof(ini.GetValue("Styling.Item.Slot.Texture", "OffsetX", ""));
	Config::Styling::Item::Slot::Texture::OffsetY = std::stof(ini.GetValue("Styling.Item.Slot.Texture", "OffsetY", ""));
	Config::Styling::Item::Slot::Texture::Scale = std::stof(ini.GetValue("Styling.Item.Slot.Texture", "Scale", ""));
	Config::Styling::Item::Slot::Text::OffsetX = std::stof(ini.GetValue("Styling.Item.Slot.Text", "OffsetX", ""));
	Config::Styling::Item::Slot::Text::OffsetY = std::stof(ini.GetValue("Styling.Item.Slot.Text", "OffsetY", ""));
	Config::Styling::Item::Slot::Text::Size = std::stoi(ini.GetValue("Styling.Item.Slot.Text", "Size", ""));
}

EventResult Config::UpdateHandler::ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource)
{
	if (!a_event) {
		return EventResult::kContinue;
	}
	if (a_event->eventName == "dmenu_updateSettings" && a_event->strArg == "Wheeler") {
		Config::ReadConfig();
	}

	return EventResult::kContinue;
}

bool Config::UpdateHandler::Register()
{
	static UpdateHandler singleton;

	auto eventSource = SKSE::GetModCallbackEventSource();

	if (!eventSource) {
		ERROR("EventSource not found!");
		return false;
	}
	eventSource->AddEventSink(&singleton);
	INFO("Register {}", typeid(singleton).name());
	return true;
}
