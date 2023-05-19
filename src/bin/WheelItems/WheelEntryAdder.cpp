#include "WheelEntryAdder.h"
#include "include/lib/Drawer.h"
#include "bin/Wheeler.h"
void WheelEntryAdder::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap)
{
	Drawer::draw_text(a_center.x, a_center.y, 0, 0, "+", 255, 255, 255, 255, 60);
}

void WheelEntryAdder::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap)
{
	Drawer::draw_text(a_center.x, a_center.y, 0, 0, "Add new slot", 255, 255, 255, 255, 30);
}

bool WheelEntryAdder::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

bool WheelEntryAdder::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

void WheelEntryAdder::ActivateItemLeft(bool editMode)
{
	if (!editMode) { // must be true
		throw std::exception("WheelEntryAdder::ActivateItemLeft: editMode must be true");
	}
}

void WheelEntryAdder::ActivateItemRight(bool editMode)
{
	if (!editMode) {  // must be true
		throw std::exception("WheelEntryAdder::ActivateItemRight: editMode must be true");
	}
	Wheeler::AddEntryToCurrentWheel();
}

void WheelEntryAdder::PrevItem()
{
}

void WheelEntryAdder::NextItem()
{
}
