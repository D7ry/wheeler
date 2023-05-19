#pragma once
#include "Config.h"
#include "imgui.h"
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

	static void TestAddItemToWheel();
	
	struct Wheel
	{
		std::vector<WheelEntry*> entries;
	};

private:
	static inline bool _active = false;
	static inline bool _editMode = false;

	static inline const char* _wheelWindowID = "##Wheeler";

	// currently active item, will be highlighted. Gets reset every time wheel reopens.
	static inline int _activeItem = -1; 
	
	static inline ImVec2 _cursorPos = { 0, 0 };

	static ImVec2 getWheelCenter();
	
	static inline std::vector<Wheel*> _wheels;
	static inline int _activeWheel = 0;
	
	// if the user presses longer than this(without sending close), the wheel will close on release
	// the the user presses shorter than this, the wheel will close on a second press.
	static inline const float _pressThreshold = .25f; 
	static inline float _openTimer = 0;

};

