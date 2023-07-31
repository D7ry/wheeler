#include "Controls.h"
#include "bin/Wheeler/Wheeler.h"
void Controls::Init()
{
	BindAllInputsFromConfig();
}

void Controls::BindAllInputsFromConfig()
{
	std::lock_guard lock(_lock);
	_keyFunctionMapDown.clear();
	_keyFunctionMapDownGamepad.clear();
	_keyFunctionMapUp.clear();
	_keyFunctionMapUpGamepad.clear();
	using namespace Config::InputBindings;
	{
		using namespace MKB;
		for (const std::pair<int, FunctionPtr>& mapping : std::vector<std::pair<int, FunctionPtr>>{
				 { activatePrimary, &Wheeler::ActivateHoveredEntryPrimary },
				 { activateSecondary, &Wheeler::ActivateHoveredEntrySecondary },
				 { addWheel, &Wheeler::AddWheel}, 
				 { addEmptyEntry, &Wheeler::AddEmptyEntryToCurrentWheel },
				 { moveEntryForward, &Wheeler::MoveEntryForwardInCurrentWheel },
				 { moveEntryBack, &Wheeler::MoveEntryBackInCurrentWheel },
				 { moveWheelForward, &Wheeler::MoveWheelForward },
				 { moveWheelBack, &Wheeler::MoveWheelBack },
				 { nextWheel, &Wheeler::NextWheel },
				 { prevWheel, &Wheeler::PrevWheel },
				 { toggleWheel, &Wheeler::ToggleWheeler },
				 { prevItem, &Wheeler::PrevItemInEntry },
				 { nextItem, &Wheeler::NextItemInEntry }
			}) {
			bindInput(mapping.first, mapping.second, true, false);
		}
		bindInput(toggleWheel, &Wheeler::CloseWheelerIfOpenedLongEnough, false, false);
	}
	{
		using namespace GamePad;
		for (const std::pair<int, FunctionPtr>& mapping : std::vector<std::pair<int, FunctionPtr>>{
				 { activatePrimary, &Wheeler::ActivateHoveredEntryPrimary },
				 { activateSecondary, &Wheeler::ActivateHoveredEntrySecondary },
				 { addWheel, &Wheeler::AddWheel },
				 { addEmptyEntry, &Wheeler::AddEmptyEntryToCurrentWheel },
				 { moveEntryForward, &Wheeler::MoveEntryForwardInCurrentWheel },
				 { moveEntryBack, &Wheeler::MoveEntryBackInCurrentWheel },
				 { moveWheelForward, &Wheeler::MoveWheelForward },
				 { moveWheelBack, &Wheeler::MoveWheelBack },
				 { nextWheel, &Wheeler::NextWheel },
				 { prevWheel, &Wheeler::PrevWheel },
				 { toggleWheel, &Wheeler::ToggleWheeler },
				 { toggleWheelIfNotInInventory, &Wheeler::ToggleWheelIfNotInInventory },
				 { toggleWheelIfInInventory, &Wheeler::ToggleWheelIfInInventory },
				 { prevItem, &Wheeler::PrevItemInEntry },
				 { nextItem, &Wheeler::NextItemInEntry } }) {
			bindInput(mapping.first, mapping.second, true, true);
		}
		bindInput(toggleWheel, &Wheeler::CloseWheelerIfOpenedLongEnough, false, true);
	}

}
void Controls::bindInput(KeyId key, FunctionPtr func, bool isDown, bool isGamePad)
{
	if (isDown) {
		if (isGamePad) {
			_keyFunctionMapDownGamepad[key] = func;
		} else {
			_keyFunctionMapDown[key] = func;
		}
		
	} else {
		if (isGamePad) {
			_keyFunctionMapUpGamepad[key] = func;
		} else {
			_keyFunctionMapUp[key] = func;
		}
		
	}
}



bool Controls::IsKeyBound(KeyId key)
{
	std::lock_guard lock(_lock);
	return _keyFunctionMapDown.contains(key) || _keyFunctionMapUp.contains(key) || _keyFunctionMapDownGamepad.contains(key) || _keyFunctionMapUpGamepad.contains(key);
}


bool Controls::Dispatch(KeyId key, bool isDown, bool isGamePad)
{
	std::lock_guard lock(_lock);
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
