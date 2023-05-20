#include "WheelEntry.h"
#include "include/lib/Drawer.h"
#include "WheelItemFactory.h"
#include "WheelItemMutableManager.h"
#include "WheelItemMutable.h"

void WheelEntry::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap)
{
	std::lock_guard<std::mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return; // nothing to draw
	}
	_items[_selectedItem]->DrawSlot(a_center, a_hovered, a_imap);
}

void WheelEntry::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap)
{
	std::lock_guard<std::mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return;  // nothing to draw
	}
	_items[_selectedItem]->DrawHighlight(a_center, a_imap);
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
	std::lock_guard<std::mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return false;  // nothing to draw
	}
	return _items[_selectedItem]->IsActive(a_inv);
}

bool WheelEntry::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	std::lock_guard<std::mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return false;  // nothing to draw
	}
	return _items[_selectedItem]->IsAvailable(a_inv);
}

void WheelEntry::ActivateItemLeft(bool editMode)
{
	std::lock_guard<std::mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return;
	}
	if (!editMode) {
		_items[_selectedItem]->ActivateItemLeft();
	} else {
		// remove selected item
		std::shared_ptr<WheelItem> itemToDelete = _items[_selectedItem];
		if (itemToDelete->IsMutable()) {
			std::shared_ptr<WheelItemMutable> mutableItem = std::dynamic_pointer_cast<WheelItemMutable>(itemToDelete);
			WheelItemMutableManager::GetSingleton()->UnTrack(mutableItem);
		}
		_items.erase(_items.begin() + _selectedItem);
		// move _selecteditem to the item immediately before the erased item
		if (_selectedItem > 0) {
			_selectedItem--;
		}
	}
}

void WheelEntry::ActivateItemRight(bool editMode)
{
	std::lock_guard<std::mutex> lock(this->_lock);

	if (!editMode) { 
		if (_items.size() == 0) {
			return;  // nothing to erase
		}
		_items[_selectedItem]->ActivateItemRight();
	} else {// append item to after _selectedItem index
		std::shared_ptr<WheelItem> newItem = WheelItemFactory::MakeWheelItemFromSelected();
		if (newItem) {
			_items.insert(_items.begin() + _selectedItem, newItem);
		}
	}
}

void WheelEntry::PrevItem()
{
	std::lock_guard<std::mutex> lock(this->_lock);

	_selectedItem--;
	if (_selectedItem < 0) {
		if (_items.size() > 1) {
			_selectedItem = _items.size() - 1;
		} else {
			_selectedItem = 0;
		}
	}
}

void WheelEntry::NextItem()
{
	std::lock_guard<std::mutex> lock(this->_lock);

	_selectedItem++;
	if (_selectedItem >= _items.size()) {
		_selectedItem = 0;
	}
}

bool WheelEntry::IsEmpty()
{
	return this->_items.empty();
}

WheelEntry::WheelEntry()
{
	_selectedItem = 0;
}

WheelEntry::~WheelEntry()
{
	for (auto& item : _items) {
		if (item->IsMutable()) {
			std::shared_ptr<WheelItemMutable> mutableItem = std::dynamic_pointer_cast<WheelItemMutable>(item);
			WheelItemMutableManager::GetSingleton()->UnTrack(mutableItem);
		}
	}
	_items.clear();
}

