#pragma once
#include <shared_mutex>
#include "nlohmann/json.hpp"
#include "imgui.h"

#include "bin/Animation/TimeInterpolator/TimeFloatInterpolator.h"
#include "bin/Animation/TimeBounceInterpolator.h"
class WheelItem;
class WheelEntry
{
public:
	WheelEntry();
	~WheelEntry();

	/// <summary>
	/// Feed in necessary information for the entry to update its interpolation animations;
	/// Called before drawing the entry.
	/// </summary>
	/// <param name="imap"></param>
	/// <param name="innerSpacingRad"></param>
	/// <param name="entryInnerAngleMin"></param>
	/// <param name="entryInnerAngleMax"></param>
	/// <param name="entryOuterAngleMin"></param>
	/// <param name="entryOuterAngleMax"></param>
	/// <param name="hovered"></param>
	void UpdateAnimation(
		RE::TESObjectREFR::InventoryItemMap& imap,
		float innerSpacingRad,
		float entryInnerAngleMin, float entryInnerAngleMax,
		float entryOuterAngleMin, float entryOuterAngleMax, bool hovered);
	/// <summary>
	/// Draw the background for this entry, including 2 arcs, one acting as the main background and other, much thinner
	/// one acting as an indicator to whether the current entry is active.
	/// The main background changes color when the entry is being hovered, and the background arc's radius linearly interpolates to an
	/// increased value.
	/// (GTA5-style)
	/// </summary>
	void DrawBackGround(const ImVec2 wheelCenter, const ImVec2 entryCenter, float innerSpacing,
		float entryInnerAngleMin, float entryInnerAngleMax,
		float entryOuterAngleMin, float entryOuterAngleMax,
		bool hovered, int numArcSegments, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawARGS);
	
	/// <summary>
	/// Draw the content in slot and (if applicable) highlight region of this wheel entry.
	/// This function should be called after DrawBackGround to prevent background from being drawn over the content.
	/// </summary> 
	void DrawSlotAndHighlight(ImVec2 a_wheelCenter, ImVec2 a_entryCenter, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs);

	/// <summary>
	/// Get the radius changes made by arcRadiusIncInterpolator. Use this function to calculate the offset of item center.
	/// </summary>
	/// <returns></returns>
	const float GetRadiusMod();

	void DrawControlPrompt(ImVec2 a_center, DrawArgs a_drawArgs);

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

	/// <summary>
	/// Activate the item with special(middle) input, which corresponds to a middle mouse click or a controller thumbstick press.
	/// </summary>
	/// <param name="editMode"></param>
	void ActivateItemSpecial(bool editMode = false);


	void PrevItem();
	void NextItem();
	void PushItem(std::shared_ptr<WheelItem> item);

	int GetSelectedItemIndex();
	void SetSelectedItem(int a_selected);

	bool IsEmpty();
	int GetNumItems();

    void SerializeIntoJsonObj(nlohmann::json& a_json);
	static std::unique_ptr<WheelEntry> SerializeFromJsonObj(const nlohmann::json& a_json, SKSE::SerializationInterface* a_intfc);

	void ResetAnimation();

private:
	void drawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs);
	void drawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs);
	
	bool _prevHovered = false;  // used to detect when the mouse enters the entry
	int _selectedItem = -1;

	std::shared_mutex _lock;
	std::vector<std::shared_ptr<WheelItem>> _items;
	
	TimeFloatInterpolator _arcRadiusIncInterpolator;  // for animating the arc radius's increase when the entry is hovered
	TimeFloatInterpolator _arcInnerAngleIncInterpolator;   // for animating the arc's angle increase when the entry is hovered
	TimeFloatInterpolator _arcOuterAngleIncInterpolator;  // for animating the arc's angle increase when the entry is hovered
	TimeBounceInterpolator _arcRadiusBounceInterpolator = TimeBounceInterpolator(0);      // for animating the arc radius's bouncing when the entry is clicked
};
