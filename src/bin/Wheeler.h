#pragma once
#include "WheelItems/WheelItem.h"
#include "Config.h"
class Wheeler
{
public:
	static Wheeler* GetInstance()
	{
		static Wheeler instance;
		return &instance;
	}
	Wheeler()
	{

	}
	void Draw();
	
	/// <summary>
	/// Load all items in the wheel customized by the current save through the serializer.
	/// This should be called on save load / on new game start.
	/// </summary>
	void LoadWheelItems();

	/// <summary>
	/// Flush all current items into a save.
	/// This should be called on game save. This is guaranteed to be called in the same save that's
	/// loaded before, so to ensure ACID.
	/// </summary>
	void FlushWheelItems();

	void OpenMenu();
	void CloseMenu();

	void ToggleEditMode();

private:
	std::vector<WheelItem*> _items;
	bool _active = false;
	bool _editMode = false;
	/// <summary>
	/// Check if wheel items are valid(existing in player inventory).
	/// If not, remove the invalid item and flush the new data.
	/// </summary>
	void verifyWheelItems();

	const char* _wheelWindowID = "##Wheeler";

	// currently active item, will be highlighted. Gets reset every time wheel reopens.
	int _activeItem = -1; 
	
	// the relative cursor within the wheel, independent of the actual mouse cursor
	ImVec2 _cursorPos = ImVec2(0, 0);
	// absolute cursor pos.
	ImVec2 _cursorPosPrevGlob = ImVec2(0, 0);
	
	void updateCursorPos();

	ImVec2 getWheelCenter();

};

