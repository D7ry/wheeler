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

	/// <summary>
	/// Update wheeler, calling draw function etc...
	/// This function can only be invoked by the renderer.
	/// </summary>
	static void Update();
	
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

	static void ToggleWheel();
	static void CloseWheelIfOpenedLongEnough();
	
	static void TryOpenWheel();
	static void TryCloseWheel();

	static void OpenWheel();
	static void CloseWheel();
	
	static void NextWheel();
	static void PrevWheel();
	static void PrevItem();
	static void NextItem();

	/// <summary>
	/// Activate the currently active entry with secondary (left) input, which corresponds to right mouse click or left controller trigger.
	/// If we're in edit mode:
	///  - the function first checks if there's any entry left. If not, the function calls DeleteCurrentWheel(), given there are more than 1 wheel present(must have at least 1 wheel on stack).
	///  - if there's some entry left, the function checks if the entry is empty.
	///		- If yes, we delete the current entry.
	///		- otherwise, the function invokes the entry's ActivateItemLeft(true) function, which handles deletion of entry's inner items.
	/// If we're not in edit mode, the entry calls the currently selected item's ActivateItemLeft()
	/// </summary>
	static void ActivateEntryLeft();

	/// <summary>
	/// Activate the currently active entry with primary (right) input, which corresponds to left mouse click or right controller trigger,
	/// The function simply invokes current entry's ActivateItemRight(), which either handles activation of items or addition of items, if in edit mode.
	/// </summary>
	static void ActivateEntryRight();

	/// <summary>
	/// Push an empty entry to the current wheel.
	/// </summary>
	static void AddEntryToCurrentWheel();

	/// <summary>
	/// Deletes the currently active entry.
	/// </summary>
	static void DeleteCurrentEntry();

	// Add a new wheel
	static void AddWheel();
	// Delete the current wheel
	static void DeleteCurrentWheel();

	static void MoveEntryForward();
	static void MoveEntryBack();

	static void MoveWheelForward();
	static void MoveWheelBack();
	
	struct Wheel
	{
		std::vector<WheelEntry*> entries;
	};
	static inline const char* SD_WHEELSWITCH = "UIFavorite";
	static inline const char* SD_ENTRYSWITCH = "UIMenuFocus";
	static inline const char* SD_WHEELERTOGGLE = "UIInventoryOpenSD";


private:
	enum class WheelState
	{
		KOpened,
		KClosed,
		KOpening,
		KClosing
	};
	static inline WheelState _state = WheelState::KClosed;
	
	static inline bool _editMode = false;

	static inline const char* _wheelWindowID = "##Wheeler";

	// currently active item, will be highlighted. Gets reset every time wheel reopens.
	static inline int _activeEntryIdx = -1; 
	
	static inline ImVec2 _cursorPos = { 0, 0 };

	static ImVec2 getWheelCenter();
	
	static inline std::vector<Wheel*> _wheels;
	static inline int _activeWheelIdx = 0;
	
	// if the user presses longer than this(without sending close), the wheel will close on release
	// the the user presses shorter than this, the wheel will close on a second press.
	static inline const float PRESS_THRESHOLD = .25f; 
	static inline float _openTimer = 0;
	static inline float _closeTimer = 0;

	static inline std::shared_mutex _wheelDataLock;  // global lock

	static bool shouldBeInEditMode(RE::UI* a_ui);

	static void hideEditModeVanillaMenus(RE::UI* a_ui);
	static void showEditModeVanillaMenus(RE::UI* a_ui);

	// Enter edit mode by pushing the adder entry to the wheel entry list. Must be called outside of the render loop
	static void enterEditMode();
	
	// Exit edit mode by popping the adder entry from the wheel entry list. Must be called outside of the render loop
	static void exitEditMode();


};

