#pragma once
#include <unordered_map>
#include <functional>
#include "Wheeler.h"
#include <unordered_set>
class Controls
{
public:
	using KeyId = uint32_t;
	using FunctionPtr = void (*)();
	static void Init();
	static void BindInput(KeyId key, FunctionPtr func, bool isDown = true);
	static void Dispatch(KeyId key, bool isDown = true);
	static bool IsKeyBound(KeyId key);

private:

	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapDown;
	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapUp;


};
