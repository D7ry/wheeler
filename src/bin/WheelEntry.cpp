#include "WheelEntry.h"
#include "include/lib/Drawer.h"
#include "WheelItemFactory.h"
#include "WheelItemMutableManager.h"
#include "WheelItemMutable.h"
#include "include/lib/Drawer.h"

void WheelEntry::Draw(const ImVec2 wheelCenter, float innerSpacing,
 float entryInnerAngleMin, 
	float entryInnerAngleMax,
 float entryOuterAngleMin, 
	float entryOuterAngleMax, const ImVec2 itemCenter,
 bool hovered, 
	int numArcSegments, RE::TESObjectREFR::InventoryItemMap& inv)
{
	using namespace Config::Styling::Wheel;

	float mainArcOuterBoundRadius = OuterCircleRadius - InnerSpacing;
	mainArcOuterBoundRadius += _arcRadiusIncInterpolator.GetValue();

	float entryInnerAngleMinUpdated = entryInnerAngleMin - _arcInnerAngleIncInterpolator.GetValue();
	float entryInnerAngleMaxUpdated = entryInnerAngleMax + _arcInnerAngleIncInterpolator.GetValue();
	float entryOuterAngleMinUpdated = entryOuterAngleMin - _arcOuterAngleIncInterpolator.GetValue();
	float entryOuterAngleMaxUpdated = entryOuterAngleMax + _arcOuterAngleIncInterpolator.GetValue();
	
	Drawer::draw_arc_gradient(wheelCenter,
		InnerCircleRadius + InnerSpacing,
		mainArcOuterBoundRadius,
		entryInnerAngleMinUpdated, entryInnerAngleMaxUpdated,
		entryOuterAngleMinUpdated, entryOuterAngleMaxUpdated,
		hovered ? HoveredColorBegin : UnhoveredColorBegin,
		hovered ? HoveredColorEnd : UnhoveredColorEnd,
		numArcSegments);

	bool active = this->IsActive(inv);
	ImU32 arcColorBegin = active ? ActiveArcColorBegin : InActiveArcColorBegin;
	ImU32 arcColorEnd = active ? ActiveArcColorEnd : InActiveArcColorEnd;

	Drawer::draw_arc_gradient(wheelCenter,
		mainArcOuterBoundRadius,
		mainArcOuterBoundRadius + ActiveArcWidth,
		entryOuterAngleMinUpdated,
		entryOuterAngleMaxUpdated,
		entryOuterAngleMinUpdated,
		entryOuterAngleMaxUpdated,
		arcColorBegin,
		arcColorEnd,
		numArcSegments);

	if (hovered) {
		this->DrawHighlight(wheelCenter, inv);
		if (!_prevHovered) {
			_arcRadiusIncInterpolator.InterpolateTo(20, 0.2f);
			_arcOuterAngleIncInterpolator.InterpolateTo(innerSpacing * (InnerCircleRadius / OuterCircleRadius), 0.2f);
			_arcInnerAngleIncInterpolator.InterpolateTo(innerSpacing, 0.2f);
			_prevHovered = true;
		}
	} else {
		_prevHovered = false;
		_arcRadiusIncInterpolator.InterpolateTo(0, 0.2f);
		_arcOuterAngleIncInterpolator.InterpolateTo(0, 0.2f);
		_arcInnerAngleIncInterpolator.InterpolateTo(0, 0.2f);
	}
	this->DrawSlot(itemCenter, hovered, inv);
}

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
		std::shared_ptr<WheelItem> newItem = WheelItemFactory::MakeWheelItemFromMenuHovered();
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
	if (Config::Sound::ItemSwitchSound && _items.size() > 1) {
		RE::PlaySoundRE(SD_ITEMSWITCH);
	}
}

void WheelEntry::NextItem()
{
	std::lock_guard<std::mutex> lock(this->_lock);

	_selectedItem++;
	if (_selectedItem >= _items.size()) {
		_selectedItem = 0;
	}
	if (Config::Sound::ItemSwitchSound && _items.size() > 1) {
		RE::PlaySoundRE(SD_ITEMSWITCH);
	}
}

bool WheelEntry::IsEmpty()
{
	return this->_items.empty();
}

std::vector<std::shared_ptr<WheelItem>>& WheelEntry::GetItems()
{
	return this->_items;
}

WheelEntry::WheelEntry()
{
	_selectedItem = 0;
}

void WheelEntry::PushItem(std::shared_ptr<WheelItem> item)
{
	this->_items.push_back(item);
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

int WheelEntry::GetSelectedItem()
{
	return this->_selectedItem;
}

void WheelEntry::SetSelectedItem(int a_selected)
{
	this->_selectedItem = a_selected;
}

