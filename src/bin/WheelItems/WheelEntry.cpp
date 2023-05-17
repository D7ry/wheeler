#include "WheelEntry.h"
#include "include/lib/Drawer.h"

void WheelEntry::DrawSlot(ImVec2 a_center, bool a_hovered)
{
	_items[_selectedItem]->DrawSlot(a_center, a_hovered);
}

void WheelEntry::DrawHighlight(ImVec2 a_center)
{
	_items[_selectedItem]->DrawHighlight(a_center);
	if (_items.size() > 1) {
		Drawer::draw_text(
			a_center.x,
			a_center.y,
			Config::Styling::Entry::Highlight::Text::OffsetX,
			Config::Styling::Entry::Highlight::Text::OffsetY,
			fmt::format("{} / {}", _selectedItem + 1, _items.size()).data(),
			255, 255, 255, 255,
			Config::Styling::Entry::Highlight::Text::Size
			);
	}
}

bool WheelEntry::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	if (_selectedItem < 0) {
		return false;
	}
	return _items[_selectedItem]->IsActive(a_inv);
}

bool WheelEntry::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	if (_selectedItem < 0) {
		return false;
	}
	return _items[_selectedItem]->IsAvailable(a_inv);
}

void WheelEntry::ActivateItemLeft()
{
	if (_selectedItem < 0) {
		return;
	}
	_items[_selectedItem]->ActivateItemLeft();
}

void WheelEntry::ActivateItemRight()
{
	if (_selectedItem < 0) {
		return;
	}
	_items[_selectedItem]->ActivateItemRight();
}

void WheelEntry::PrevItem()
{
	_selectedItem--;
	if (_selectedItem < 0) {
		_selectedItem = _items.size() - 1;
	}
}

void WheelEntry::NextItem()
{
	_selectedItem++;
	if (_selectedItem >= _items.size()) {
		_selectedItem = 0;
	}
}

