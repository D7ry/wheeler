#include "bin/Rendering/Drawer.h"

#include "WheelItems/WheelItem.h"
#include "WheelItems/WheelItemFactory.h"
#include "WheelItems/WheelItemMutable.h"
#include "WheelEntry.h"
#include "WheelItems/WheelItemShout.h"
void WheelEntry::UpdateAnimation(RE::TESObjectREFR::InventoryItemMap& imap, float innerSpacingRad, float entryInnerAngleMin, float entryInnerAngleMax, float entryOuterAngleMin, float entryOuterAngleMax, bool hovered)
{
	using namespace Config::Styling::Wheel;
	bool active = this->IsActive(imap);
	if (hovered) {
		if (!_prevHovered) {
			float expandSize = Config::Animation::EntryHighlightExpandScale * (OuterCircleRadius - InnerCircleRadius);
			_arcRadiusIncInterpolator.InterpolateTo(expandSize, Config::Animation::EntryHighlightExpandTime);
			_arcOuterAngleIncInterpolator.InterpolateTo(innerSpacingRad * (InnerCircleRadius / OuterCircleRadius), Config::Animation::EntryHighlightExpandTime);
			_arcInnerAngleIncInterpolator.InterpolateTo(innerSpacingRad, Config::Animation::EntryHighlightExpandTime);
			_prevHovered = true;
		}
	} else {
		if (_prevHovered != false) {
			_prevHovered = false;
			_arcRadiusIncInterpolator.InterpolateTo(0, Config::Animation::EntryHighlightRetractTime);
			_arcOuterAngleIncInterpolator.InterpolateTo(0, Config::Animation::EntryHighlightRetractTime);
			_arcInnerAngleIncInterpolator.InterpolateTo(0, Config::Animation::EntryHighlightRetractTime);
		}
	}
}

void WheelEntry::DrawBackGround(
	const ImVec2 wheelCenter, const ImVec2 entryCenter, 
	float innerSpacingRad, 
	float entryInnerAngleMin, float entryInnerAngleMax,
	float entryOuterAngleMin, float entryOuterAngleMax, 
	bool hovered, 
	int numArcSegments, RE::TESObjectREFR::InventoryItemMap& inv, DrawArgs a_drawARGS)
{
	bool active = this->IsActive(inv);
	//TODO:
	// 1. add separate config for background texture scaling
	// 2. decouple this function

	using namespace Config::Styling::Wheel;

	if (UseGeometricPrimitiveForBackgroundTexture) {
		float mainArcOuterBoundRadius = OuterCircleRadius;
		mainArcOuterBoundRadius += _arcRadiusIncInterpolator.GetValue();
		mainArcOuterBoundRadius += _arcRadiusBounceInterpolator.GetValue();

		float entryInnerAngleMinUpdated = entryInnerAngleMin - _arcInnerAngleIncInterpolator.GetValue() * 2;
		float entryInnerAngleMaxUpdated = entryInnerAngleMax + _arcInnerAngleIncInterpolator.GetValue() * 2;
		float entryOuterAngleMinUpdated = entryOuterAngleMin - _arcOuterAngleIncInterpolator.GetValue() * 2;
		float entryOuterAngleMaxUpdated = entryOuterAngleMax + _arcOuterAngleIncInterpolator.GetValue() * 2;

		Drawer::draw_arc_gradient(wheelCenter,
			InnerCircleRadius,
			mainArcOuterBoundRadius,
			entryInnerAngleMinUpdated, entryInnerAngleMaxUpdated,
			entryOuterAngleMinUpdated, entryOuterAngleMaxUpdated,
			hovered ? HoveredColorBegin : UnhoveredColorBegin,
			hovered ? HoveredColorEnd : UnhoveredColorEnd,
			numArcSegments, a_drawARGS);

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
			numArcSegments, a_drawARGS);
	} else {
		Texture::icon_image_type backgroundImageType = Texture::icon_image_type::slot_background;
		if (active) {
			backgroundImageType = Texture::icon_image_type::slot_active_background;
		} else if (hovered) {
			backgroundImageType = Texture::icon_image_type::slot_highlighted_background;
		}
		Texture::Image backgroundTexture = Texture::GetIconImage(backgroundImageType);
		
		Drawer::draw_texture(backgroundTexture.texture, entryCenter, 0, 0,
			ImVec2(backgroundTexture.width * Config::Styling::Item::Slot::BackgroundTexture::Scale
				, backgroundTexture.height * Config::Styling::Item::Slot::BackgroundTexture::Scale), C_SKYRIMWHITE, a_drawARGS);
	
	}

	
}

void WheelEntry::DrawSlotAndHighlight(ImVec2 a_wheelCenter, ImVec2 a_entryCenter, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	try {

		if (a_hovered) {
			this->drawHighlight(a_wheelCenter, a_imap, a_drawArgs);
		}
		this->drawSlot(a_entryCenter, a_hovered, a_imap, a_drawArgs);
	} catch (std::exception& e) {
		logger::error("Exception in WheelEntry::DrawSlotAndHighlight: {}", e.what());
	}
}

/*这个也能用，但是库存数据过多可能会导致卡顿掉帧，猜的。先留着吧
void WheelEntry::drawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	try {
		std::shared_lock<std::shared_mutex> lock(this->_lock);

		if (_items.size() == 0) {
			return;  // nothing to draw
		}

		std::vector<int> indicesToDelete; 
		for (int i = 0; i < _items.size(); ++i) {
			auto item = _items[i];
			if (!item->IsAvailable(a_imap)) {  
				indicesToDelete.push_back(i);
			}
		}

		for (auto it = indicesToDelete.rbegin(); it != indicesToDelete.rend(); ++it) {
			_items.erase(_items.begin() + *it);
			if (*it < _selectedItem && _selectedItem > 0) {
				_selectedItem--;  
			}
		}

		if (_items.size() == 0) {
			return;
		}

		if (_selectedItem >= _items.size()) {
			_selectedItem = _items.size() - 1; 
		}

		_items[_selectedItem]->DrawSlot(a_center, a_hovered, a_imap, a_drawArgs);
	} catch (const std::exception& e) {
		logger::error("Exception in WheelEntry::drawSlot: {}", e.what());
	}
}
*/
void WheelEntry::drawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	try {
		std::shared_lock<std::shared_mutex> lock(this->_lock);

		if (_items.empty()) {
			return;  // nothing to draw
		}

		bool itemRemoved = false;

		//遍历所有的 slot，检查物品是否还在玩家库存中
		for (int i = 0; i < _items.size(); ++i) {
			auto item = _items[i];
			//如果是 shout，跳过可用性检查
			if (auto shoutItem = std::dynamic_pointer_cast<WheelItemShout>(item)) {
				continue;  //不要删除 shout
			}
			if (!item->IsAvailable(a_imap)) {
				//标记删除的物品，不立即删除以防止影响遍历过程
				_items.erase(_items.begin() + i);
				if (i < _selectedItem && _selectedItem > 0) {
					_selectedItem--;
				}
				i--;  //调整索引，防止跳过下一个元素
				itemRemoved = true;
			}
		}

		//如果没有可用的物品，直接返回
		if (_items.empty()) {
			return;
		}

		//确保 _selectedItem 在有效范围内
		if (_selectedItem >= _items.size()) {
			_selectedItem = _items.size() - 1;  //防止越界
		}

		//绘制slot
		_items[_selectedItem]->DrawSlot(a_center, a_hovered, a_imap, a_drawArgs);
	} catch (const std::exception& e) {
		logger::error("Exception in WheelEntry::drawSlot: {}", e.what());
	}
}

void WheelEntry::drawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	try {
		std::shared_lock<std::shared_mutex> lock(this->_lock);

		if (_items.size() == 0) {
			return;  // nothing to draw
		}
		_items[_selectedItem]->DrawHighlight(a_center, a_imap, a_drawArgs);
		if (_items.size() > 1) {
			Drawer::draw_text(
				a_center.x + Config::Styling::Entry::Highlight::Text::OffsetX,
				a_center.y + Config::Styling::Entry::Highlight::Text::OffsetY,
				fmt::format("{} / {}", _selectedItem + 1, _items.size()).data(),
				C_SKYRIMWHITE,
				Config::Styling::Entry::Highlight::Text::Size,
				a_drawArgs);
		}
	} catch (std::exception& e) {
		logger::error("Exception in WheelEntry::drawHighlight: {}", e.what());
	}
}

const float WheelEntry::GetRadiusMod()
{
	return this->_arcRadiusIncInterpolator.GetValue() + _arcRadiusBounceInterpolator.GetValue();
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
		_arcRadiusBounceInterpolator.InterpolateTo(Config::Animation::EntryInputBumpScale * (Config::Styling::Wheel::OuterCircleRadius - Config::Styling::Wheel::InnerCircleRadius), Config::Animation::EntryInputBumpTime);
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
			return;  // nothing to do
		}
		_items[_selectedItem]->ActivateItemPrimary();
		_arcRadiusBounceInterpolator.InterpolateTo(Config::Animation::EntryInputBumpScale * (Config::Styling::Wheel::OuterCircleRadius - Config::Styling::Wheel::InnerCircleRadius), Config::Animation::EntryInputBumpTime);
	} else {// append item to after _selectedItem index
		std::shared_ptr<WheelItem> newItem = WheelItemFactory::MakeWheelItemFromMenuHovered();
		if (newItem) {
			_items.insert(_items.begin() + _selectedItem, newItem);
		}
	}
}

void WheelEntry::ActivateItemSpecial(bool editMode)
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);
	if (editMode || _items.size() == 0) {
		return; // nothing to do
	}
	_items[_selectedItem]->ActivateItemSpecial();
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
	if (_items.size() > 1) {
#undef PlaySound

		RE::PlaySound(Config::Sound::SD_ITEMSWITCH);

#ifdef UNICODE
#	define PlaySound PlaySoundW
#else
#	define PlaySound PlaySoundA
#endif  // !UNICODE
	}
}

void WheelEntry::NextItem()
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);

	_selectedItem++;
	if (_selectedItem >= _items.size()) {
		_selectedItem = 0;
	}
	if (_items.size() > 1) {
#undef PlaySound

		RE::PlaySound(Config::Sound::SD_ITEMSWITCH);

#ifdef UNICODE
#	define PlaySound PlaySoundW
#else
#	define PlaySound PlaySoundA
#endif  // !UNICODE
	}
}

bool WheelEntry::IsEmpty()
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);
	return this->_items.empty();
}

int WheelEntry::GetNumItems()
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);
	return this->_items.size();
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

int WheelEntry::GetSelectedItemIndex()
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

	nlohmann::json j_items = j_entry["items"];
	try {
		for (const auto& j_item : j_items) {
			std::shared_ptr<WheelItem> item = WheelItemFactory::MakeWheelItemFromJsonObject(j_item, a_intfc);
			if (item) {
				entry->PushItem(std::move(item));
			}
		}
	}
	catch (std::exception exception) {
		logger::info("Exception serializing wheel entry: {}", exception.what());
	}
	int selectedItem = std::clamp(j_entry["selecteditem"].get<int>(), 0, entry->GetNumItems());
	entry->SetSelectedItem(selectedItem);

	return std::move(entry);
}

void WheelEntry::ResetAnimation()
{
	_arcInnerAngleIncInterpolator.ForceFinish();
	_arcOuterAngleIncInterpolator.ForceFinish();
	_arcRadiusIncInterpolator.ForceFinish();
	_arcInnerAngleIncInterpolator.ForceValue(0);
	_arcOuterAngleIncInterpolator.ForceValue(0);
	_arcRadiusIncInterpolator.ForceValue(0);
	// don't need call interpolateto() because bounce interpolator's forceFinish() goes back to its fixed starting value (which is where we want it to be at)
	_arcRadiusBounceInterpolator.ForceFinish();
	this->_prevHovered = false;
}


void WheelEntry::SetSelectedItem(int a_selected)
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);
	this->_selectedItem = a_selected;
}

