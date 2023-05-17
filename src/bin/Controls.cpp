#include "Controls.h"
#include "Wheeler.h"
void Controls::Init()
{
	for (const auto pair :
		std::vector<std::pair<KeyId, FunctionPtr>>
		{
			{ 0x10, &Wheeler::NextWheel }, // e
			{ 0x12, &Wheeler::PrevWheel }, // q
			{ 79, &Wheeler::OpenMenu },// numpad 1
			{ 80, &Wheeler::CloseMenu }, // numpad 2
			{ 81, &Wheeler::ToggleEditMode }, // numpad 3
			{ 264, &Wheeler::PrevItem }, // mouse wheel up
			{ 265, &Wheeler::NextItem }, // mouse wheel down
			{ 256, &Wheeler::ActivateItemLeft}, // mouse left
			{ 257, &Wheeler::ActivateItemRight} // mouse right
		}
		) {
		BindInput(pair.first, pair.second);
	}
}
void Controls::BindInput(KeyId key, FunctionPtr func)
{
	if (_functionKeyMap.contains(func)) {
		KeyId oldKey = _functionKeyMap[func];
		_keyFunctionMap.erase(oldKey);
	}
	_functionKeyMap[func] = key;
	_keyFunctionMap[key] = func;
}

bool Controls::IsKeyBound(KeyId key)
{
	return _keyFunctionMap.contains(key);
}

void Controls::Dispatch(KeyId key)
{
	if (_keyFunctionMap.contains(key)) {
		FunctionPtr func = _keyFunctionMap[key];
		func();
	}
}
