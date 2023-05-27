#pragma once
#include "WheelItemMutable.h"
#include "bin/Animation/TimeColorInterpolator.h"

class TimeColorInterpolator;
class WheelItemWeapon : public WheelItemMutable 
{
public:
	void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap) override;
	void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap) override;
	bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	WheelItemWeapon(RE::TESBoundObject* a_weapon, uint16_t a_uniqueID);

	~WheelItemWeapon()
	{
		// wheelitemmutable's destructor will remove it from the manager
	};
	void ActivateItemSecondary() override;
	void ActivateItemPrimary() override;
	
	virtual void SerializeIntoJsonObj(nlohmann::json& a_json) override;


	static inline const char* ITEM_TYPE_STR = "WheelItemWeapon";

private:
	void equipItem(bool a_toRight = true);
	TimeColorInterpolator _testInterpolator;
};
