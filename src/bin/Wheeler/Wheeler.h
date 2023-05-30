#pragma once
#include <mutex>
#include <shared_mutex>
#include "nlohmann/json.hpp"
#include "imgui.h"

#include "bin/Config.h"
#include "Wheel.h"
class Wheeler
{
public:
	static void Init()
	{
		// insert an empty wheel
		_wheels.emplace_back(std::make_unique<Wheel>());
	}

	/// <summary>
	/// Update wheeler, calling draw function etc...
	/// This function can only be invoked by the renderer.
	/// </summary>
	static void Update(float a_deltaTime);
	
	/// <summary>
	/// Resets everything, must be called prior to reloading the wheels.
	/// </summary>
	static void Clear();

	static void UpdateCursorPosMouse(float a_deltaX, float a_deltaY);
	static void UpdateCursorPosGamepad(float a_x, float a_y);

	static void ToggleWheeler();
	static void CloseWheelerIfOpenedLongEnough();
	
	static void TryOpenWheeler();
	static void TryCloseWheeler();

	static void OpenWheeler();
	static void CloseWheeler();
	
	static void NextWheel();
	static void PrevWheel();
	static void PrevItemInEntry();
	static void NextItemInEntry();

	/// <summary>
	/// Activate the currently active entry with secondary (left) input, which corresponds to right mouse click or left controller trigger.
	/// If we're in edit mode:
	///  - the function first checks if there's any entry left. If not, the function calls DeleteCurrentWheel(), given there are more than 1 wheel present(must have at least 1 wheel on stack).
	///  - if there's some entry left, the function checks if the entry is empty.
	///		- If yes, we delete the current entry.
	///		- otherwise, the function invokes the entry's ActivateItemSecondary(true) function, which handles deletion of entry's inner items.
	/// If we're not in edit mode, the entry calls the currently selected item's ActivateItemSecondary()
	/// </summary>
	static void ActivateHoveredEntrySecondary();

	/// <summary>
	/// Activate the currently active entry with primary (right) input, which corresponds to left mouse click or right controller trigger,
	/// The function simply invokes current entry's ActivateItemPrimary(), which either handles activation of items or addition of items, if in edit mode.
	/// </summary>
	static void ActivateHoveredEntryPrimary();

	static void ActivateHoveredEntrySpecial();

	/// <summary>
	/// Push an empty entry to the current wheel.
	/// </summary>
	static void AddEmptyEntryToCurrentWheel();

	// Add a new wheel
	static void AddWheel();
	// Delete the current wheel
	static void DeleteCurrentWheel();

	static void MoveEntryForwardInCurrentWheel();
	static void MoveEntryBackInCurrentWheel();

	static void MoveWheelForward();
	static void MoveWheelBack();
	
	static int GetActiveWheelIndex();
	static void SetActiveWheelIndex(int a_index);
	

	static bool IsWheelerOpen();
	static bool IsInEditMode();

	static void SerializeFromJsonObj(const nlohmann::json& a_json, SKSE::SerializationInterface* a_intfc);
	static void SerializeIntoJsonObj(nlohmann::json& a_json);

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

	
	static inline ImVec2 _cursorPos = { 0, 0 };

	static ImVec2 getWheelCenter();
	
	static inline std::vector<std::unique_ptr<Wheel>> _wheels;
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

