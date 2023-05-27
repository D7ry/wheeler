#pragma once
#include <unordered_map>
#include <functional>
#include <unordered_set>
class Controls
{
public:
	using KeyId = uint32_t;
	using FunctionPtr = void (*)();
	static void Init();
	static void BindInput(KeyId key, FunctionPtr func, bool isDown = true, bool isGamePad = false);
	static void Dispatch(KeyId key, bool isDown = true, bool isGamePad = false);
	static bool IsKeyBound(KeyId key);

private:

	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapDown;
	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapUp;

	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapDownGamepad;
	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapUpGamepad;
};
