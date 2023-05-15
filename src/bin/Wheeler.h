#pragma once
#include "WheelItems/WheelItem.h"
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
};

namespace WheelerStyling
{
	inline float RADIUS_MIN = 150.0f;
	inline float RADIUS_MAX = 360.0f;
	inline int ITEMS_MIN = 1;
	inline float ITEM_INNER_SPACING = 5.f;
}
