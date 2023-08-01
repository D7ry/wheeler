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
	/// Resets everything, freeing wheels and their memebers in the hierarchy.
	/// must be called prior to reloading the wheels.
	/// </summary>
	static void Clear();

	static void UpdateCursorPosMouse(float a_deltaX, float a_deltaY);
	static void UpdateCursorPosGamepad(float a_x, float a_y);

	static void ToggleWheeler();
	static void ToggleWheelIfInInventory();
	static void ToggleWheelIfNotInInventory();

	/// <summary>
	/// Close the current wheel, if it's been opened long enough(more than 0.2 seconds).
	/// This collaborates with `Controls` that invokes this function when the user releases the wheel toggle key.
	/// Correct usage of this function and ToggleWheeler() ensures the following behavior:
	/// 	When the user presses down the toggle key for the first time, wheeler opens.
	///		If the user immediately releases the key, wheeler stays open, until the user presses the toggle key again.
	///		If the user keeps pressing the toggle key for a while and then releases the key, wheeler automatically closes.
	/// This allows the toggle key to simultaneously act as a press-open, press-close toggle, and a hold-open, release-close button.
	/// </summary>
	static void CloseWheelerIfOpenedLongEnough();
	static void CloseWheelerIfOpenedLongEnoughIfInInventory();
	static void CloseWheelerIfOpenedLongEnoughIfNotInInventory();

	
	static void TryOpenWheeler();
	static void TryCloseWheeler();

	static void OpenWheeler();
	static void CloseWheeler();
	
	static void NextWheel();
	static void PrevWheel();
	static void PrevItemInEntry();
	static void NextItemInEntry();

	static bool GetCursorAngleRadian(float& r_ret);

	/// <summary>
	/// Offset camera rotation with current cusor position. Returns whether a change has been made to the camera's rotation.
	/// </summary>
	static bool OffsetCamera(RE::TESCamera* a_this) = delete;

	/// <summary>
	/// Activate the currently active entry with secondary (left) input, which corresponds to right mouse click or left controller trigger.
	/// If we're in edit mode:
	///  - the function first checks if there's any entry left. If not, the function calls DeleteCurrentWheel(), given there are more than 1 wheel present(must have at least 1 wheel on stack).
	///  - if there's some entry left, the function calls the current wheel's ActivateItemSecondary(), which handles deletion of entry items, or the entry via subsequent calls.
	/// If we're not in edit mode, the entry calls the current wheel's ActivateItemSecondary()
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

	/// <summary>
	/// Add a new empty wheel to the set of wheels.
	/// </summary>
	static void AddWheel();

	/// <summary>
	/// Delete the current wheel. The deletion may be performed if and only if the current wheel is empty, and the current wheel is not the last wheel present.
	/// The caller is responsible for checking the wheel's emptiness.
	/// </summary>
	static void DeleteCurrentWheel();

	/// <summary>
	/// Move the currently active entry forward by one in the current wheel. Only available in edit mode.
	/// </summary>
	static void MoveEntryForwardInCurrentWheel();
	/// <summary>
	/// Move the currently active entry backward by one in the current wheel. Only available in edit mode.
	/// </summary>
	static void MoveEntryBackInCurrentWheel();
	
	/// <summary>
	/// Move the currently active wheel forward by one in the set of wheels. Only available in edit mode.
	/// </summary>
	static void MoveWheelForward();
	/// <summary>
	/// Move the currently active wheel backward by one in the set of wheels. Only available in edit mode.
	/// </summary>
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

	


	static inline float _openTimer = 0;
	static inline float _closeTimer = 0;

	static inline std::shared_mutex _wheelDataLock;  // global lock

	// Whether the wheel should enter edit mode. Edit mode toggles whenever a game inventory UI opens up.
	static bool shouldBeInEditMode(RE::UI* a_ui);
	
	static void hideEditModeVanillaMenus(RE::UI* a_ui);
	static void showEditModeVanillaMenus(RE::UI* a_ui);

	static void enterEditMode();
	static void exitEditMode();

	static float getCursorRadiusMax();
};

