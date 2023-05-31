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

	/// <summary>
	/// Find the action for the corresponding keyID and down/up condition, dispatching the function is found.
	/// </summary>
	/// <param name="key">id of the key input</param>
	/// <param name="isDown">whether the key is being pressed down or being relesaed</param>
	/// <param name="isGamePad">whether the input comes from game pad</param>
	/// <returns>whether the dispatch is successful.</returns>
	static bool Dispatch(KeyId key, bool isDown = true, bool isGamePad = false);

	
	static bool IsKeyBound(KeyId key);
	
private:

	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapDown;
	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapUp;

	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapDownGamepad;
	static inline std::unordered_map<KeyId, FunctionPtr> _keyFunctionMapUpGamepad;

	//static inline KeyId _keyToggleWheelGamepad = 280; // left trigger
	//static inline KeyId _keyToggleWheelMKB = 58; // caps lock
};
