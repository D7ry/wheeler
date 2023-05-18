#pragma once
#include "WheelItem.h"
class WheelItemWeapon : public WheelItem 
{
public:
	void DrawSlot(ImVec2 a_center, bool a_hovered) override;
	void DrawHighlight(ImVec2 a_center) override;
	bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	WheelItemWeapon(RE::TESObjectWEAP* a_weapon, RE::EnchantmentItem* a_enchant = nullptr, float a_health = -1.f);

	void ActivateItemLeft() override;
	void ActivateItemRight() override;

private:
	RE::TESObjectWEAP* _weapon = nullptr;
	RE::TESBoundObject* _weaponBound = nullptr;
	float _weaponHealth = -1.f;
	RE::EnchantmentItem* _enchant = nullptr;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="a_inv"></param>
	/// <returns></returns>
	std::pair<RE::TESBoundObject*, int> getInvBoundObject(RE::TESObjectREFR::InventoryItemMap& a_inv);
	void equipItem(bool a_toRight = true);
};
