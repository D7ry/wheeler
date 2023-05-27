#pragma once
#include "bin/Rendering/TextureManager.h"
#include "bin/Config.h"
#include "nlohmann/json.hpp"
class ImVec2;
class WheelItem
{
public:
	WheelItem(){};
	WheelItem(Texture::Image tex)
	{
		_texture = tex;
	}
	// draw the item on the wheel's slot
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, float a_alphaMult = 1.0f);
	// draw the item on the wheel's highlight region(given the item is highlighted) e.g. the center of the wheel(GTA-V ish)
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, float a_alphaMult = 1.0f);
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv);

	/// <summary>
	/// Whether the item is available. Unavailable items are greyed out and are typically
	/// due to previous bound item being removed from inventory.
	/// </summary>
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv);

	virtual void ActivateItemSecondary();
	virtual void ActivateItemPrimary();

	virtual bool IsMutable();

	virtual void SerializeIntoJsonObj(nlohmann::json& a_json);
	static std::shared_ptr<WheelItem> SerializeFromJsonObj(nlohmann::json& a_json);

	static inline const char* ITEM_TYPE_STR = "WheelItem";


protected:
	Texture::Image _texture;

	

};
