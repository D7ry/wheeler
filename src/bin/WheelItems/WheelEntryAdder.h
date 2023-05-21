#pragma once
#include "WheelEntry.h"

/// <summary>
/// Deprecated class
/// </summary>
class WheelEntryAdder : public WheelEntry
{
private:
	static WheelEntryAdder* GetSingleton()
	{
		static WheelEntryAdder singleton;
		return &singleton;
	}
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap) override;
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap) override;
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual void ActivateItemLeft(bool editMode = false) override;
	virtual void ActivateItemRight(bool editMode = false) override;
	virtual void PrevItem() override;
	virtual void NextItem() override;
};
