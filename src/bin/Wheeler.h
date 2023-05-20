#pragma once
#include "Config.h"
#include "imgui.h"
#include <mutex>
#include <shared_mutex>

class WheelItem;
class WheelItemMutable;
class WheelEntry;
class Wheeler
{
public:
	static void Init()
	{
		// insert an empty wheel
		_wheels.emplace_back(new Wheel());
	}
	static void Draw();
	
	/// <summary>
	/// Load all items in the wheel customized by the current save through the serializer.
	/// This should be called on save load / on new game start.
	/// </summary>
	static void LoadWheelItems();

	/// <summary>
	/// Flush all current items into a save.
	/// This should be called on game save. This is guaranteed to be called in the same save that's
	/// loaded before, so to ensure ACID.
	/// </summary>
	static void FlushWheelItems();

	static void UpdateCursorPosMouse(float a_deltaX, float a_deltaY);
	static void UpdateCursorPosGamepad(float a_x, float a_y);

	static void ToggleMenu();
	static void CloseMenuIfOpenedLongEnough();
	static void OpenMenu();
	static void CloseMenu();
	
	static void NextWheel();
	static void PrevWheel();
	static void PrevItem();
	static void NextItem();
	static void ActivateItemLeft();
	static void ActivateItemRight();

	static void AddEntryToCurrentWheel();

	/// <summary>
	/// This functions behaves differently depending on the current state of the wheel.
	/// If the currently hovered over item is a regular entry, the regular entry is guaranteed to have
	/// an empty list of items(ensured by ActivateItemLeft), we delete the hovered-over entry
	/// If the currently hovered over item is an Adder entry, we delete the wheel iff:
	/// 1. the wheel is empty execept for the adder entry.
	/// 2. the wheel is not the last remaining wheel
	/// </summary>
	static void DeleteCurrent();

	// Add a new wheel
	static void AddWheel();
	// Delete the current wheel
	static void DeleteCurrentWheel();
	struct Wheel
	{
		std::vector<WheelEntry*> entries;
	};

private:
	static inline bool _active = false;
	static inline bool _editMode = false;

	static inline const char* _wheelWindowID = "##Wheeler";

	// currently active item, will be highlighted. Gets reset every time wheel reopens.
	static inline int _activeEntry = -1; 
	
	static inline ImVec2 _cursorPos = { 0, 0 };

	static ImVec2 getWheelCenter();
	
	static inline std::vector<Wheel*> _wheels;
	static inline int _activeWheel = 0;
	
	// if the user presses longer than this(without sending close), the wheel will close on release
	// the the user presses shorter than this, the wheel will close on a second press.
	static inline const float _pressThreshold = .25f; 
	static inline float _openTimer = 0;

	static inline std::shared_mutex _wheelDataLock;  // global lock

	static bool shouldBeInEditMode(RE::UI* a_ui);

	static void hideEditModeVanillaMenus(RE::UI* a_ui);
	static void showEditModeVanillaMenus(RE::UI* a_ui);

	// Enter edit mode by pushing the adder entry to the wheel entry list. Must be called outside of the render loop
	static void enterEditMode();
	
	// Exit edit mode by popping the adder entry from the wheel entry list. Must be called outside of the render loop
	static void exitEditMode();
};

