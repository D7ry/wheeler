#pragma once
#include <unordered_map>
#include <functional>
#include "Wheeler.h"
#include <unordered_set>
class Controls
{
public:
	using FunctionPtr = void (*)();
	static void Init();
	static void BindInput(uint32_t key, FunctionPtr func);
	static void Dispatch(uint32_t key);
	static void AddPressedKey(uint32_t key);
	static void RemovePressedKey(uint32_t key);
	static void FlushPressedKeys();
	static std::unordered_set<uint32_t>& GetPressedKeys();

private:
	struct FunctionPtrHash
	{
		std::size_t operator()(FunctionPtr func) const
		{
			return std::hash<void (*)()>()(func);
		}
	};
	// doubly linked map for value-based lookup
	static inline std::unordered_map<FunctionPtr, uint32_t, FunctionPtrHash> _functionKeyMap;
	static inline std::unordered_map<uint32_t, FunctionPtr> _keyFunctionMap;


	static inline std::unordered_set<uint32_t> _pressedKeys;


};
