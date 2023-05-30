#include "Controls.h"
#include "bin/Wheeler/Wheeler.h"
void Controls::Init()
{
	//https://www.creationkit.com/index.php?title=Input_Script
	// MKB
	for (const auto pair :
		std::vector<std::pair<KeyId, FunctionPtr>>
		{
			{ 0x12, &Wheeler::NextWheel }, // e
			{ 0x10, &Wheeler::PrevWheel }, // q
			{ 58, &Wheeler::ToggleWheeler },  // caps lock
			{ 264, &Wheeler::PrevItemInEntry }, // mouse wheel up
			{ 265, &Wheeler::NextItemInEntry }, // mouse wheel down
			{ 256, &Wheeler::ActivateHoveredEntryPrimary}, // mouse left
			{ 257, &Wheeler::ActivateHoveredEntrySecondary}, // mouse right
			{ 49, &Wheeler::AddWheel }, // N
			{ 50, &Wheeler::AddEmptyEntryToCurrentWheel }, // M
			{ 200, &Wheeler::MoveEntryForwardInCurrentWheel }, // up arrow
			{ 208, &Wheeler::MoveEntryBackInCurrentWheel }, // down arrow
			{ 203, &Wheeler::MoveWheelBack },          // left arrow
			{ 205, &Wheeler::MoveWheelForward },       // right arrow
			{ 258, &Wheeler::ActivateHoveredEntrySpecial }      // mouse middle
		}
		) {
		BindInput(pair.first, pair.second, true, false);
	}
	for (const auto pair :
		std::vector<std::pair<KeyId, FunctionPtr>>{
			{ 58, &Wheeler::CloseWheelerIfOpenedLongEnough },  // caps lock
		}) {
		BindInput(pair.first, pair.second, false, false);
	}
	// gamepad
	for (const auto pair :
		std::vector<std::pair<KeyId, FunctionPtr>>{
			{ 281, &Wheeler::NextWheel },  // right trigger
			//{ 0x10, &Wheeler::PrevWheel },                      // q
			{ 280, &Wheeler::ToggleWheeler },                   // left trigger
			{ 268, &Wheeler::PrevItemInEntry },                 // DPAD left
			{ 269, &Wheeler::NextItemInEntry },                 // DPAD right
			{ 275, &Wheeler::ActivateHoveredEntryPrimary },    // right shoulder
			{ 274, &Wheeler::ActivateHoveredEntrySecondary },   // left shoulder
			{ 273, &Wheeler::ActivateHoveredEntrySpecial }     // right thumb
		}) {
		BindInput(pair.first, pair.second, true, true);
	}
	for (const auto pair :
		std::vector<std::pair<KeyId, FunctionPtr>>{
			{ 280, &Wheeler::CloseWheelerIfOpenedLongEnough },  // LEFT SHOULDER
		}) {
		BindInput(pair.first, pair.second, false, true);
	}
}
void Controls::BindInput(KeyId key, FunctionPtr func, bool isDown, bool isGamePad)
{
	// do a linear search for the old key(can't have 2 keys bound to the same function)
	if (isDown) {
		if (isGamePad) {
			for (auto& [k, f] : _keyFunctionMapDownGamepad) {
				if (f == func) {
					_keyFunctionMapDownGamepad.erase(k);
					break;
				}
			}
			_keyFunctionMapDownGamepad[key] = func;
		} else {
			for (auto& [k, f] : _keyFunctionMapDown) {
				if (f == func) {
					_keyFunctionMapDown.erase(k);
					break;
				}
			}
			_keyFunctionMapDown[key] = func;
		}
		
	} else {
		if (isGamePad) {
			for (auto& [k, f] : _keyFunctionMapUpGamepad) {
				if (f == func) {
					_keyFunctionMapUpGamepad.erase(k);
					break;
				}
			}
			_keyFunctionMapUpGamepad[key] = func;
		} else {
			for (auto& [k, f] : _keyFunctionMapUp) {
				if (f == func) {
					_keyFunctionMapUp.erase(k);
					break;
				}
			}
			_keyFunctionMapUp[key] = func;
		}
		
	}
	
}

bool Controls::IsKeyBound(KeyId key)
{
	return _keyFunctionMapDown.contains(key) || _keyFunctionMapUp.contains(key) || _keyFunctionMapDownGamepad.contains(key) || _keyFunctionMapUpGamepad.contains(key);
}

bool Controls::Dispatch(KeyId key, bool isDown, bool isGamePad)
{
	if (isDown) {
		if (isGamePad) {
			if (_keyFunctionMapDownGamepad.contains(key)) {
				FunctionPtr func = _keyFunctionMapDownGamepad[key];
				func();
				return true;
			}
		} else {
			if (_keyFunctionMapDown.contains(key)) {
				FunctionPtr func = _keyFunctionMapDown[key];
				func();
				return true;
			}
		}

	} else {
		if (isGamePad) {
			if (_keyFunctionMapUpGamepad.contains(key)) {
				FunctionPtr func = _keyFunctionMapUpGamepad[key];
				func();
				return true;
			}
		} else {
			if (_keyFunctionMapUp.contains(key)) {
				FunctionPtr func = _keyFunctionMapUp[key];
				func();
				return true;
			}
		}
	}
	return false;
}
