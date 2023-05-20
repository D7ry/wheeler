#pragma once
#include "WheelItem.h"
#include <vector>

class WheelEntry
{
	// this don't inherit WheelItem because it is a container of that
public:
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap);
	
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap);

	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv);
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv);

	virtual void ActivateItemLeft(bool editMode = false);
	virtual void ActivateItemRight(bool editMode = false);

	virtual void PrevItem();
	virtual void NextItem();

	bool IsEmpty();

	WheelEntry();

	~WheelEntry();
	std::vector<std::shared_ptr<WheelItem>> _items;

private:
	int _selectedItem;
	std::mutex _lock;
};
