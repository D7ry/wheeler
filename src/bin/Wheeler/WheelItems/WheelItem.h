#pragma once
#include "bin/Rendering/TextureManager.h"
#include "bin/Config.h"
#include "nlohmann/json.hpp"
class ImVec2;
class WheelItem
{
public:
	WheelItem(){};
	
	/// <summary>
	/// Draw everything that's supposed to be in a wheel slot(entry)
	/// Currently, DrawSlot should draw an image of the item and its name.
	/// </summary>
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs);
	
	/// <summary>
	/// Draw everything of the item that's supposed to be in the highlight region i.e. center of the wheel.
	/// Currently, DrawHighlight should draw an enlarged image of the item, item description, and item stats(if applicable).
	/// </summary>
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs);
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv);

	/// <summary>
	/// Whether the item is available. 
	/// An item is unavailable when the player lacks skill to use it, or the item is not in the player's inventory.
	/// </summary>
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv);

	virtual void ActivateItemPrimary();
	virtual void ActivateItemSecondary();
	virtual void ActivateItemSpecial();

	virtual void SerializeIntoJsonObj(nlohmann::json& a_json);
	static std::shared_ptr<WheelItem> SerializeFromJsonObj(nlohmann::json& a_json);

	static inline const char* ITEM_TYPE_STR = "WheelItem";


protected:
	Texture::Image _texture = Texture::Image();
	std::string _description = "";  // buffer for description.
	

};
