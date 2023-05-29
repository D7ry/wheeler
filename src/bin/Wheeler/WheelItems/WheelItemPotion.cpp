#include "WheelItemPotion.h"

void WheelItemPotion::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
}

void WheelItemPotion::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
}

bool WheelItemPotion::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

bool WheelItemPotion::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

void WheelItemPotion::ActivateItemPrimary()
{
}

void WheelItemPotion::ActivateItemSecondary()
{
}

void WheelItemPotion::ActivateItemSpecial()
{
}

void WheelItemPotion::SerializeIntoJsonObj(nlohmann::json& a_json)
{
}
