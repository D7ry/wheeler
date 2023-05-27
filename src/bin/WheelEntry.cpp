#include "WheelEntry.h"

#include "include/lib/Drawer.h"
#include "WheelItems/WheelItem.h"
#include "WheelItems/WheelItemFactory.h"
#include "WheelItems/WheelItemMutable.h"
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

	float entryInnerAngleMinUpdated = entryInnerAngleMin - _arcInnerAngleIncInterpolator.GetValue() * 2;
	float entryInnerAngleMaxUpdated = entryInnerAngleMax + _arcInnerAngleIncInterpolator.GetValue() * 2;
	float entryOuterAngleMinUpdated = entryOuterAngleMin - _arcOuterAngleIncInterpolator.GetValue() * 2;
	float entryOuterAngleMaxUpdated = entryOuterAngleMax + _arcOuterAngleIncInterpolator.GetValue() * 2;
	
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
		if (_prevHovered != false) {
			_prevHovered = false;
			_arcRadiusIncInterpolator.InterpolateTo(0, 0.1f);
			_arcOuterAngleIncInterpolator.InterpolateTo(0, 0.1f);
			_arcInnerAngleIncInterpolator.InterpolateTo(0, 0.1f);
		}
	}
	this->DrawSlot(itemCenter, hovered, inv);
}

void WheelEntry::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap)
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return; // nothing to draw
	}
	_items[_selectedItem]->DrawSlot(a_center, a_hovered, a_imap);
}

void WheelEntry::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap)
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);

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
	std::shared_lock<std::shared_mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return false;  // nothing to draw
	}
	return _items[_selectedItem]->IsActive(a_inv);
}

bool WheelEntry::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return false;  // nothing to draw
	}
	return _items[_selectedItem]->IsAvailable(a_inv);
}

void WheelEntry::ActivateItemSecondary(bool editMode)
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);

	if (_items.size() == 0) {
		return;
	}
	if (!editMode) {
		_items[_selectedItem]->ActivateItemSecondary();
	} else {
		// remove selected item
		std::shared_ptr<WheelItem> itemToDelete = _items[_selectedItem];
		_items.erase(_items.begin() + _selectedItem);
		// move _selecteditem to the item immediately before the erased item
		if (_selectedItem > 0) {
			_selectedItem--;
		}
	}
}

void WheelEntry::ActivateItemPrimary(bool editMode)
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);

	if (!editMode) { 
		if (_items.size() == 0) {
			return;  // nothing to erase
		}
		_items[_selectedItem]->ActivateItemPrimary();
	} else {// append item to after _selectedItem index
		std::shared_ptr<WheelItem> newItem = WheelItemFactory::MakeWheelItemFromMenuHovered();
		if (newItem) {
			_items.insert(_items.begin() + _selectedItem, newItem);
		}
	}
}

void WheelEntry::PrevItem()
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);

	_selectedItem--;
	if (_selectedItem < 0) {
		if (_items.size() > 1) {
			_selectedItem = _items.size() - 1;
		} else {
			_selectedItem = 0;
		}
	}
	if (Config::Sound::ItemSwitchSound && _items.size() > 1) {
		RE::PlaySoundRE(Config::Sound::SD_ITEMSWITCH);
	}
}

void WheelEntry::NextItem()
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);

	_selectedItem++;
	if (_selectedItem >= _items.size()) {
		_selectedItem = 0;
	}
	if (Config::Sound::ItemSwitchSound && _items.size() > 1) {
		RE::PlaySoundRE(Config::Sound::SD_ITEMSWITCH);
	}
}

bool WheelEntry::IsEmpty()
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);
	return this->_items.empty();
}

std::vector<std::shared_ptr<WheelItem>>& WheelEntry::GetItems()
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);
	return this->_items;
}

WheelEntry::WheelEntry()
{
	_selectedItem = 0;
}

void WheelEntry::PushItem(std::shared_ptr<WheelItem> item)
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);
	this->_items.push_back(item);
}

WheelEntry::~WheelEntry()
{
	_items.clear();
}

int WheelEntry::GetSelectedItem()
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);
	return this->_selectedItem;
}

void WheelEntry::SerializeIntoJsonObj(nlohmann::json& j_entry)
{
	// setup for entry
	j_entry["items"] = nlohmann::json::array();
	for (std::shared_ptr<WheelItem> item : this->_items) {
		nlohmann::json j_item;
		item->SerializeIntoJsonObj(j_item);
		j_entry["items"].push_back(j_item);
	}
	j_entry["selecteditem"] = this->_selectedItem;
}

std::unique_ptr<WheelEntry> WheelEntry::SerializeFromJsonObj(const nlohmann::json& j_entry, SKSE::SerializationInterface* a_intfc)
{
	std::unique_ptr<WheelEntry> entry = std::make_unique<WheelEntry>();
	entry->SetSelectedItem(j_entry["selecteditem"]);

	nlohmann::json j_items = j_entry["items"];
	for (const auto& j_item : j_items) {
		std::shared_ptr<WheelItem> item = WheelItemFactory::MakeWheelItemFromJsonObject(j_item, a_intfc);
		if (item) {
			entry->PushItem(std::move(item));
		}
	}
	return std::move(entry);
}

void WheelEntry::ResetAnimation()
{
	_arcInnerAngleIncInterpolator.ForceValue(0);
	_arcOuterAngleIncInterpolator.ForceValue(0);
	_arcRadiusIncInterpolator.ForceValue(0);
}


void WheelEntry::SetSelectedItem(int a_selected)
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);
	this->_selectedItem = a_selected;
}

