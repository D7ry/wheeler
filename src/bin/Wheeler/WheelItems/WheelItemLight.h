#pragma once
#include "WheelItem.h"
class WheelItemLight : public WheelItem
{
public:
	WheelItemLight() = delete;
	WheelItemLight(RE::TESObjectLIGH* a_light);
	
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs) override;
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs) override;
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual void ActivateItemSecondary() override;
	virtual void ActivateItemPrimary() override;

	virtual void SerializeIntoJsonObj(nlohmann::json& a_json) override;

	virtual RE::TESForm* GetItemForm() override;

	static inline const char* ITEM_TYPE_STR = "WheelItemLight";

private:
	RE::TESObjectLIGH* _light;
	
	void toggleEquip();
};
