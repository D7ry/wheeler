#include "Controls.h"
void Controls::Init()
{
	for (const auto pair :
		std::vector<std::pair<KeyId, FunctionPtr>>
		{
			{ 0x10, &Wheeler::NextWheel },
			{ 0x12, &Wheeler::PrevWheel },
			{ 79, &Wheeler::OpenMenu },
			{ 80, &Wheeler::CloseMenu },
			{ 81, &Wheeler::ToggleEditMode }
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

void Controls::Dispatch(KeyId key)
{
	if (_keyFunctionMap.contains(key)) {
		FunctionPtr func = _keyFunctionMap[key];
		func();
	}
}
void Controls::AddPressedKey(KeyId key)
{
	_pressedKeys.insert(key);
}
void Controls::RemovePressedKey(KeyId key)
{
	_pressedKeys.erase(key);
}
void Controls::FlushPressedKeys()
{
	_pressedKeys.clear();
}
std::unordered_set<uint32_t>& Controls::GetPressedKeys()
{
	return _pressedKeys;
}
