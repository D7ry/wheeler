#include "WheelItem.h"
#include "bin/Rendering/Drawer.h"
void WheelItem::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	//Drawer::draw_element(_texture, ImVec2(0, 0), ImVec2(100, 100), 0);
}

void WheelItem::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
}


bool WheelItem::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

bool WheelItem::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

void WheelItem::ActivateItemSecondary()
{
}

void WheelItem::ActivateItemPrimary()
{
}

void WheelItem::ActivateItemSpecial()
{
}

bool WheelItem::IsMutable()
{
	return false;
}

void WheelItem::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = ITEM_TYPE_STR;
	a_json["WARNING"] = "This is a placeholder item. It should not be used.";
}

