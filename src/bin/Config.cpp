#include "Config.h"
#include "UserInput/Controls.h"

#define SETTINGSFILE_PATH "Data\\SKSE\\Plugins\\wheeler\\Settings.ini"

bool GetBoolValue(const CSimpleIniA& ini, const char* section, const char* key, bool& value)
{
	const char* strValue = ini.GetValue(section, key, nullptr);
	if (strValue != nullptr) {
		value = ini.GetBoolValue(section, key, value);
		return true;
	}
	return false;
}

bool GetUInt32Value(const CSimpleIniA& ini, const char* section, const char* key, uint32_t& value)
{
	const char* strValue = ini.GetValue(section, key, nullptr);
	if (strValue != nullptr) {
		try {
			value = static_cast<uint32_t>(std::stoul(strValue));
			return true;
		} catch (const std::invalid_argument&) {
			ERROR("Failed to parse {}: {} when reading uint32 value.", section, key);
			return false;
		}
	}
	return false;
}

bool GetFloatValue(const CSimpleIniA& ini, const char* section, const char* key, float& value)
{
	const char* strValue = ini.GetValue(section, key, nullptr);
	if (strValue != nullptr) {
		try {
			value = std::stof(strValue);
			return true;
		} catch (const std::invalid_argument&) {
			// Conversion failed, value remains unchanged
			ERROR("Failed to parse {}: {} when reading float value.", section, key);
			return false;
		}
	}
	return false;
}
void Config::ReadConfig()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(SETTINGSFILE_PATH);

	GetFloatValue(ini, "Control.Wheel", "CursorRadiusPerEntry", Config::Control::Wheel::CursorRadiusPerEntry);
	GetBoolValue(ini, "Control.Wheel", "DoubleActivateDisable", Config::Control::Wheel::DoubleActivateDisable);
	GetFloatValue(ini, "Control.Wheel", "ToggleHoldThreshold", Config::Control::Wheel::ToggleHoldThreshold);
	GetFloatValue(ini, "Styling.Wheel", "CursorIndicatorDist", Config::Styling::Wheel::CursorIndicatorDist);
	GetFloatValue(ini, "Styling.Wheel", "CusorIndicatorArcWidth", Config::Styling::Wheel::CusorIndicatorArcWidth);
	GetFloatValue(ini, "Styling.Wheel", "CursorIndicatorArcAngle", Config::Styling::Wheel::CursorIndicatorArcAngle);
	GetFloatValue(ini, "Styling.Wheel", "CursorIndicatorTriangleSideLength", Config::Styling::Wheel::CursorIndicatorTriangleSideLength);
	GetUInt32Value(ini, "Styling.Wheel", "CursorIndicatorColor", Config::Styling::Wheel::CursorIndicatorColor);
	GetFloatValue(ini, "Styling.Wheel", "WheelIndicatorOffsetX", Config::Styling::Wheel::WheelIndicatorOffsetX);
	GetFloatValue(ini, "Styling.Wheel", "WheelIndicatorOffsetY", Config::Styling::Wheel::WheelIndicatorOffsetY);
	GetFloatValue(ini, "Styling.Wheel", "WheelIndicatorSize", Config::Styling::Wheel::WheelIndicatorSize);
	GetFloatValue(ini, "Styling.Wheel", "WheelIndicatorSpacing", Config::Styling::Wheel::WheelIndicatorSpacing);
	GetUInt32Value(ini, "Styling.Wheel", "WheelIndicatorActiveColor", Config::Styling::Wheel::WheelIndicatorActiveColor);
	GetUInt32Value(ini, "Styling.Wheel", "WheelIndicatorInactiveColor", Config::Styling::Wheel::WheelIndicatorInactiveColor);
	GetFloatValue(ini, "Styling.Wheel", "InnerCircleRadius", Config::Styling::Wheel::InnerCircleRadius);
	GetFloatValue(ini, "Styling.Wheel", "OuterCircleRadius", Config::Styling::Wheel::OuterCircleRadius);
	GetFloatValue(ini, "Styling.Wheel", "InnerSpacing", Config::Styling::Wheel::InnerSpacing);

	GetUInt32Value(ini, "Styling.Wheel", "HoveredColorBegin", Config::Styling::Wheel::HoveredColorBegin);
	GetUInt32Value(ini, "Styling.Wheel", "HoveredColorEnd", Config::Styling::Wheel::HoveredColorEnd);
	GetUInt32Value(ini, "Styling.Wheel", "UnhoveredColorBegin", Config::Styling::Wheel::UnhoveredColorBegin);
	GetUInt32Value(ini, "Styling.Wheel", "UnhoveredColorEnd", Config::Styling::Wheel::UnhoveredColorEnd);
	GetUInt32Value(ini, "Styling.Wheel", "ActiveArcColorBegin", Config::Styling::Wheel::ActiveArcColorBegin);
	GetUInt32Value(ini, "Styling.Wheel", "ActiveArcColorEnd", Config::Styling::Wheel::ActiveArcColorEnd);
	GetUInt32Value(ini, "Styling.Wheel", "InActiveArcColorBegin", Config::Styling::Wheel::InActiveArcColorBegin);
	GetUInt32Value(ini, "Styling.Wheel", "InActiveArcColorEnd", Config::Styling::Wheel::InActiveArcColorEnd);
	GetFloatValue(ini, "Styling.Wheel", "ActiveArcWidth", Config::Styling::Wheel::ActiveArcWidth);
	GetBoolValue(ini, "Styling.Wheel", "BlurOnOpen", Config::Styling::Wheel::BlurOnOpen);
	GetFloatValue(ini, "Styling.Wheel", "SlowTimeScale", Config::Styling::Wheel::SlowTimeScale);
	GetFloatValue(ini, "Styling.Wheel", "FadeTime", Config::Styling::Wheel::FadeTime);
	GetFloatValue(ini, "Styling.Wheel", "CenterOffsetX", Config::Styling::Wheel::CenterOffsetX);
	GetFloatValue(ini, "Styling.Wheel", "CenterOffsetY", Config::Styling::Wheel::CenterOffsetY);

	GetFloatValue(ini, "Styling.Entry.Highlight.Text", "OffsetX", Config::Styling::Entry::Highlight::Text::OffsetX);
	GetFloatValue(ini, "Styling.Entry.Highlight.Text", "OffsetY", Config::Styling::Entry::Highlight::Text::OffsetY);
	GetFloatValue(ini, "Styling.Entry.Highlight.Text", "Size", Config::Styling::Entry::Highlight::Text::Size);

	GetFloatValue(ini, "Styling.Item.Highlight.Texture", "OffsetX", Config::Styling::Item::Highlight::Texture::OffsetX);
	GetFloatValue(ini, "Styling.Item.Highlight.Texture", "OffsetY", Config::Styling::Item::Highlight::Texture::OffsetY);
	GetFloatValue(ini, "Styling.Item.Highlight.Texture", "Scale", Config::Styling::Item::Highlight::Texture::Scale);
	GetFloatValue(ini, "Styling.Item.Highlight.Text", "OffsetX", Config::Styling::Item::Highlight::Text::OffsetX);
	GetFloatValue(ini, "Styling.Item.Highlight.Text", "OffsetY", Config::Styling::Item::Highlight::Text::OffsetY);
	GetFloatValue(ini, "Styling.Item.Highlight.Text", "Size", Config::Styling::Item::Highlight::Text::Size);
	GetFloatValue(ini, "Styling.Item.Highlight.Desc", "OffsetX", Config::Styling::Item::Highlight::Desc::OffsetX);
	GetFloatValue(ini, "Styling.Item.Highlight.Desc", "OffsetY", Config::Styling::Item::Highlight::Desc::OffsetY);
	GetFloatValue(ini, "Styling.Item.Highlight.Desc", "Size", Config::Styling::Item::Highlight::Desc::Size);
	GetFloatValue(ini, "Styling.Item.Highlight.Desc", "LineLength", Config::Styling::Item::Highlight::Desc::LineLength);
	GetFloatValue(ini, "Styling.Item.Highlight.Desc", "LineSpacing", Config::Styling::Item::Highlight::Desc::LineSpacing);

	GetFloatValue(ini, "Styling.Item.Slot.Texture", "OffsetX", Config::Styling::Item::Slot::Texture::OffsetX);
	GetFloatValue(ini, "Styling.Item.Slot.Texture", "OffsetY", Config::Styling::Item::Slot::Texture::OffsetY);
	GetFloatValue(ini, "Styling.Item.Slot.Texture", "Scale", Config::Styling::Item::Slot::Texture::Scale);
	GetFloatValue(ini, "Styling.Item.Slot.Text", "OffsetX", Config::Styling::Item::Slot::Text::OffsetX);
	GetFloatValue(ini, "Styling.Item.Slot.Text", "OffsetY", Config::Styling::Item::Slot::Text::OffsetY);
	GetFloatValue(ini, "Styling.Item.Slot.Text", "Size", Config::Styling::Item::Slot::Text::Size);

	GetFloatValue(ini, "Animation", "EntryHighlightExpandTime", Config::Animation::EntryHighlightExpandTime);
	GetFloatValue(ini, "Animation", "EntryHighlightRetractTime", Config::Animation::EntryHighlightRetractTime);
	GetFloatValue(ini, "Animation", "EntryHighlightExpandScale", Config::Animation::EntryHighlightExpandScale);
	GetFloatValue(ini, "Animation", "EntryInputBumpScale", Config::Animation::EntryInputBumpScale);
	GetFloatValue(ini, "Animation", "EntryInputBumpTime", Config::Animation::EntryInputBumpTime);

	GetUInt32Value(ini, "InputBindings.GamePad", "nextWheel", Config::InputBindings::GamePad::nextWheel);
	GetUInt32Value(ini, "InputBindings.GamePad", "prevWheel", Config::InputBindings::GamePad::prevWheel);
	GetUInt32Value(ini, "InputBindings.GamePad", "toggleWheel", Config::InputBindings::GamePad::toggleWheel);
	GetUInt32Value(ini, "InputBindings.GamePad", "nextItem", Config::InputBindings::GamePad::nextItem);
	GetUInt32Value(ini, "InputBindings.GamePad", "prevItem", Config::InputBindings::GamePad::prevItem);
	GetUInt32Value(ini, "InputBindings.GamePad", "activatePrimary", Config::InputBindings::GamePad::activatePrimary);
	GetUInt32Value(ini, "InputBindings.GamePad", "activateSecondary", Config::InputBindings::GamePad::activateSecondary);
	GetUInt32Value(ini, "InputBindings.GamePad", "addWheel", Config::InputBindings::GamePad::addWheel);
	GetUInt32Value(ini, "InputBindings.GamePad", "addEmptyEntry", Config::InputBindings::GamePad::addEmptyEntry);
	GetUInt32Value(ini, "InputBindings.GamePad", "moveEntryForward", Config::InputBindings::GamePad::moveEntryForward);
	GetUInt32Value(ini, "InputBindings.GamePad", "moveEntryBack", Config::InputBindings::GamePad::moveEntryBack);
	GetUInt32Value(ini, "InputBindings.GamePad", "moveWheelForward", Config::InputBindings::GamePad::moveWheelForward);
	GetUInt32Value(ini, "InputBindings.GamePad", "moveWheelBack", Config::InputBindings::GamePad::moveWheelBack);

	GetUInt32Value(ini, "InputBindings.MKB", "nextWheel", Config::InputBindings::MKB::nextWheel);
	GetUInt32Value(ini, "InputBindings.MKB", "prevWheel", Config::InputBindings::MKB::prevWheel);
	GetUInt32Value(ini, "InputBindings.MKB", "toggleWheel", Config::InputBindings::MKB::toggleWheel);
	GetUInt32Value(ini, "InputBindings.MKB", "nextItem", Config::InputBindings::MKB::nextItem);
	GetUInt32Value(ini, "InputBindings.MKB", "prevItem", Config::InputBindings::MKB::prevItem);
	GetUInt32Value(ini, "InputBindings.MKB", "activatePrimary", Config::InputBindings::MKB::activatePrimary);
	GetUInt32Value(ini, "InputBindings.MKB", "activateSecondary", Config::InputBindings::MKB::activateSecondary);
	GetUInt32Value(ini, "InputBindings.MKB", "addWheel", Config::InputBindings::MKB::addWheel);
	GetUInt32Value(ini, "InputBindings.MKB", "addEmptyEntry", Config::InputBindings::MKB::addEmptyEntry);
	GetUInt32Value(ini, "InputBindings.MKB", "moveEntryForward", Config::InputBindings::MKB::moveEntryForward);
	GetUInt32Value(ini, "InputBindings.MKB", "moveEntryBack", Config::InputBindings::MKB::moveEntryBack);
	GetUInt32Value(ini, "InputBindings.MKB", "moveWheelForward", Config::InputBindings::MKB::moveWheelForward);
	GetUInt32Value(ini, "InputBindings.MKB", "moveWheelBack", Config::InputBindings::MKB::moveWheelBack);
	//GetBoolValue(ini, "Animation", "Camera Rotation", Config::Animation::CameraRotation);
}

EventResult Config::UpdateHandler::ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource)
{
	if (!a_event) {
		return EventResult::kContinue;
	}
	if (a_event->eventName == "dmenu_updateSettings" && a_event->strArg == "Wheeler") {
		Config::ReadConfig();
		Controls::BindAllInputsFromConfig();
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
