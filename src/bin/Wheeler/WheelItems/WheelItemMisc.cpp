#include "bin/Rendering/TextureManager.h"
#include "bin/Rendering/Drawer.h"
#include "WheelItemMisc.h"

WheelItemMisc::WheelItemMisc(RE::TESObjectMISC* a_miscItem)
{
	this->_miscItem = a_miscItem;
	this->_texture = Texture::GetIconImage(Texture::icon_image_type::icon_default, a_miscItem);
}

void WheelItemMisc::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	this->drawSlotText(a_center, this->_miscItem->GetName(), a_drawArgs);
	this->drawSlotTexture(a_center, a_drawArgs);
}

void WheelItemMisc::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	this->drawHighlightText(a_center, this->_miscItem->GetName(), a_drawArgs);
	this->drawHighlightTexture(a_center, a_drawArgs);
}

bool WheelItemMisc::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

bool WheelItemMisc::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return a_inv.contains(this->_miscItem) && a_inv[this->_miscItem].first > 0;
}

void WheelItemMisc::ActivateItemSecondary()
{
	this->useItem();
}

void WheelItemMisc::ActivateItemPrimary()
{
	this->useItem();
}

void WheelItemMisc::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = WheelItemMisc::ITEM_TYPE_STR;
	a_json["formID"] = this->_miscItem->GetFormID();
}

void WheelItemMisc::useItem()
{
	RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();
	RE::ActorEquipManager* aeMan = RE::ActorEquipManager::GetSingleton();
	if (!pc || !aeMan) {
		return;
	}
	aeMan->EquipObject(pc, this->_miscItem);
}

RE::TESForm* WheelItemMisc::GetItemForm()
{
	return this->_miscItem;
}
