#pragma once
#include "WheelItems/WheelItem.h"
#include "bin/Animation/TimeTrapezoidInterpolator.h"

class WheelEntry
{
	// this don't inherit WheelItem because it is a container of that
public:
	/**
			entry->Draw(
				entryInnerAngleMin, entryInnerAngleMax, 
				entryOuterAngleMin, entryOuterAngleMax,
				itemCenter, hovered, numArcSegments
			)
			*/
	void Draw(const ImVec2 wheelCenter, float innerSpacing,
 float entryInnerAngleMin, float entryInnerAngleMax,
 float entryOuterAngleMin, float entryOuterAngleMax,
		const ImVec2 itemCenter, bool hovered, int numArcSegments, RE::TESObjectREFR::InventoryItemMap& a_imap);
	void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap);
	
	void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap);

	bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv);
	bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv);

	/// <summary>
	/// Activate the item with secondary (left) input, which corresponds to right mouse click or left controller trigger.
	/// If we're in edit mode, the entry deletes the currently selected item until there are no items left.
	/// If we're not in edit mode, the entry calls the currently selected item's ActivateItemSecondary().
	/// </summary>
	/// <param name="editMode">Whether the wheel is in edit mode.</param>
	void ActivateItemSecondary(bool editMode = false);

	/// <summary>
	/// Activate the item with primary(right) input, which corresponds to left mouse click or right controller trigger.
	/// If we're in edit mode, the entry queries WheelItemFactory for a new item (the item that the cursor is hovering over in either inventory or magic menu)
	/// and pushes it to the entry's front.
	/// If we're not in edit mode, the entry calls the currently selected item's ActivateItemPrimary().
	/// </summary>
	/// <param name="editMode">Whether the wheel is in edit mode.</param>
	void ActivateItemPrimary(bool editMode = false);

	void PrevItem();
	void NextItem();

	bool IsEmpty();

	std::vector<std::shared_ptr<WheelItem>>& GetItems();

	WheelEntry();

	void PushItem(std::shared_ptr<WheelItem> item);

	~WheelEntry();

	int GetSelectedItem();
	void SetSelectedItem(int a_selected);

    void SerializeIntoJsonObj(nlohmann::json& a_json);
	static std::unique_ptr<WheelEntry*> SerializeFromJsonObj(nlohmann::json& a_json);

private:
	bool _prevHovered = false;  // used to detect when the mouse enters the entry
	int _selectedItem;
	std::shared_mutex _lock;
	static inline const char* SD_ITEMSWITCH = "UIMenuPrevNextSD"; // sound descriptor id
	std::vector<std::shared_ptr<WheelItem>> _items;
	TimeFloatInterpolator _arcRadiusIncInterpolator;  // for animating the arc radius's increase when the entry is hovered
	TimeFloatInterpolator _arcInnerAngleIncInterpolator;   // for animating the arc's angle increase when the entry is hovered
	TimeFloatInterpolator _arcOuterAngleIncInterpolator;  // for animating the arc's angle increase when the entry is hovered
};
