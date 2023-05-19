#pragma once
#include "WheelItem.h"
#include "WheelItemMutable.h"
class WheelItemWeapon : public WheelItemMutable 
{
public:
	void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap) override;
	void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap) override;
	bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	WheelItemWeapon(RE::TESObjectWEAP* a_weapon, uint16_t a_uniqueID);

	void ActivateItemLeft() override;
	void ActivateItemRight() override;

private:

	void equipItem(bool a_toRight = true);
};
