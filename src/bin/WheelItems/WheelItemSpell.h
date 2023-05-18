#pragma once
#include "WheelItem.h"
class WheelItemSpeel : public WheelItem
{
public:
	WheelItemSpeel(RE::SpellItem* a_spell);
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered) override;
	virtual void DrawHighlight(ImVec2 a_center) override;
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual void ActivateItemLeft() override;
	virtual void ActivateItemRight() override;

private:
	RE::SpellItem* _spell;
};
