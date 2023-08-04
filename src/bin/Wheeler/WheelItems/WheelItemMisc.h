#pragma once
#include "WheelItem.h"
class WheelItemMisc : public WheelItem
{
public:
	WheelItemMisc() = delete;
	WheelItemMisc(RE::TESObjectMISC* a_miscItem);

	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs) override;
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs) override;
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual void ActivateItemSecondary() override;
	virtual void ActivateItemPrimary() override;

	virtual void SerializeIntoJsonObj(nlohmann::json& a_json) override;

	static inline const char* ITEM_TYPE_STR = "WheelItemMisc";

private:
	RE::TESObjectMISC* _miscItem;

	void useItem();
};
