#pragma once
#include "WheelItem.h"
class WheelItemShout : public WheelItem
{
public:
	WheelItemShout() = delete;
	
	WheelItemShout(RE::TESShout* a_shout);
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs) override;
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs) override;
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual void ActivateItemSecondary() override;
	virtual void ActivateItemPrimary() override;
	virtual void SerializeIntoJsonObj(nlohmann::json& a_json) override;
	virtual void ActivateItemSpecial() override;

	virtual RE::TESForm* GetItemForm() override;
	
	static inline const char* ITEM_TYPE_STR = "WheelItemShout";

private:
	RE::TESShout* _shout = nullptr;
};
