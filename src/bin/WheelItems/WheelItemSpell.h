#pragma once
#include "WheelItem.h"
class WheelItemSpeel : public WheelItem // spell AND power
{
public:
	WheelItemSpeel(RE::SpellItem* a_spell);
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap) override;
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap) override;
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual void ActivateItemLeft() override;
	virtual void ActivateItemRight() override;

private:
	RE::SpellItem* _spell = nullptr;
};
