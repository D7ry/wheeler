#pragma once
#include "WheelItem.h"
#include <vector>

class WheelEntry
{
	// this don't inherit WheelItem because it is a container of that
public:
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap);
	
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap);

	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv);
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv);

	/// <summary>
	/// Activate the item with secondary (left) input, which corresponds to right mouse click or left controller trigger.
	/// If we're in edit mode, the entry deletes the currently selected item until there are no items left.
	/// If we're not in edit mode, the entry calls the currently selected item's ActivateItemLeft().
	/// </summary>
	/// <param name="editMode">Whether the wheel is in edit mode.</param>
	virtual void ActivateItemLeft(bool editMode = false);

	/// <summary>
	/// Activate the item with primary(right) input, which corresponds to left mouse click or right controller trigger.
	/// If we're in edit mode, the entry queries WheelItemFactory for a new item (the item that the cursor is hovering over in either inventory or magic menu)
	/// and pushes it to the entry's front.
	/// If we're not in edit mode, the entry calls the currently selected item's ActivateItemRight().
	/// </summary>
	/// <param name="editMode">Whether the wheel is in edit mode.</param>
	virtual void ActivateItemRight(bool editMode = false);

	virtual void PrevItem();
	virtual void NextItem();

	bool IsEmpty();

	std::vector<std::shared_ptr<WheelItem>>& GetItems();

	WheelEntry();

	~WheelEntry();
	std::vector<std::shared_ptr<WheelItem>> _items;

	int GetSelectedItem();

private:
	int _selectedItem;
	std::mutex _lock;
	static inline const char* SD_ITEMSWITCH = "UIMenuPrevNextSD";
};
