#include "Input.h"

#include <WinUser.h>
#include <Windows.h>
#include <dinput.h>

#include <imgui.h>


#include "bin/Wheeler/Wheeler.h"
#include "Controls.h"
#define IM_VK_KEYPAD_ENTER (VK_RETURN + 256)

class CharEvent : public RE::InputEvent
{
public:
	uint32_t keyCode;  // 18 (ascii code)
};


static enum : std::uint32_t
{
	kInvalid = static_cast<std::uint32_t>(-1),
	kKeyboardOffset = 0,
	kMouseOffset = 256,
	kGamepadOffset = 266
};
			
static inline std::uint32_t GetGamepadIndex(RE::BSWin32GamepadDevice::Key a_key)
{
	using Key = RE::BSWin32GamepadDevice::Key;

	std::uint32_t index;
	switch (a_key) {
	case Key::kUp:
		index = 0;
		break;
	case Key::kDown:
		index = 1;
		break;
	case Key::kLeft:
		index = 2;
		break;
	case Key::kRight:
		index = 3;
		break;
	case Key::kStart:
		index = 4;
		break;
	case Key::kBack:
		index = 5;
		break;
	case Key::kLeftThumb:
		index = 6;
		break;
	case Key::kRightThumb:
		index = 7;
		break;
	case Key::kLeftShoulder:
		index = 8;
		break;
	case Key::kRightShoulder:
		index = 9;
		break;
	case Key::kA:
		index = 10;
		break;
	case Key::kB:
		index = 11;
		break;
	case Key::kX:
		index = 12;
		break;
	case Key::kY:
		index = 13;
		break;
	case Key::kLeftTrigger:
		index = 14;
		break;
	case Key::kRightTrigger:
		index = 15;
		break;
	default:
		index = kInvalid;
		break;
	}

	return index != kInvalid ? index + kGamepadOffset : kInvalid;
}
bool Input::ProcessAndDeny(RE::InputEvent* const* a_event)
{
	if (!a_event) {
		return true;
	}
	bool shouldDispatch = true;
	bool wheelerOpen = Wheeler::IsWheelerOpen();
	for (auto event = *a_event; event; event = event->next) {
		auto eventType = event->eventType;
		// update cursor pos only if the wheel is open, and hence block dispatch of thumbstick/mouse event to game
		if (event->eventType == RE::INPUT_EVENT_TYPE::kMouseMove) {
			if (wheelerOpen) {
				RE::MouseMoveEvent* mouseMove = static_cast<RE::MouseMoveEvent*>(event);
				Wheeler::UpdateCursorPosMouse(mouseMove->mouseInputX, mouseMove->mouseInputY);
				shouldDispatch = false; // block mouse input when wheel is open
			}
		} else if (event->eventType == RE::INPUT_EVENT_TYPE::kThumbstick) {
			if (wheelerOpen) {
				RE::ThumbstickEvent* thumbstick = static_cast<RE::ThumbstickEvent*>(event);
				if (thumbstick->IsRight()) {
					Wheeler::UpdateCursorPosGamepad(thumbstick->xValue, thumbstick->yValue);
					shouldDispatch = false;  // block thumbstick right input when wheel is open
				}
			}
		} else if (event->eventType == RE::INPUT_EVENT_TYPE::kButton) {
			const auto button = static_cast<RE::ButtonEvent*>(event);
			if (!button || (button->IsPressed() && !button->IsDown()))
				continue;

			auto scan_code = button->GetIDCode();

			using DeviceType = RE::INPUT_DEVICE;
			bool isGamePad = false;
			auto input = scan_code;
			switch (button->device.get()) {
			case DeviceType::kMouse:
				input += kMouseOffset;
				break;
			case DeviceType::kKeyboard:
				input += kKeyboardOffset;
				break;
			case DeviceType::kGamepad:
				input = GetGamepadIndex((RE::BSWin32GamepadDevice::Key)input);
				isGamePad = true;
				break;
			default:
				continue;
			}

			// block button inputs to game when when the input is bound, and when the wheel is open.
			if (button->IsDown()) {
				if (Controls::Dispatch(input, true, isGamePad) && wheelerOpen) {
					shouldDispatch = false;
				}
			} else if (button->IsUp()) {
				if (Controls::Dispatch(input, false, isGamePad) && wheelerOpen) {
					shouldDispatch = false;
				}
			}

		}
	}

	return shouldDispatch;
}

