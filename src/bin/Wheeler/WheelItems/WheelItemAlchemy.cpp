#include "bin/Rendering/Drawer.h"
#include "WheelItemAlchemy.h"

WheelItemAlchemy::WheelItemAlchemy(RE::AlchemyItem* a_alchemyItem)
{
	this->_alchemyItem = a_alchemyItem;
	Texture::icon_image_type iconType = Texture::icon_image_type::food;
	if (_alchemyItem->data.flags.any(RE::AlchemyItem::AlchemyFlag::kFoodItem) 
		|| _alchemyItem->HasKeywordString("VendorItemFood")) {
		iconType = Texture::icon_image_type::food;
	} else if (_alchemyItem->data.flags.any(RE::AlchemyItem::AlchemyFlag::kMedicine) 
		|| _alchemyItem->HasKeywordString("VendorItemPotion")) {
		iconType = Texture::icon_image_type::potion_default;
	} else if (_alchemyItem->data.flags.any(RE::AlchemyItem::AlchemyFlag::kPoison) 
		|| _alchemyItem->HasKeywordString("VendorItemPoison")) {
		iconType = Texture::icon_image_type::poison_default;
	}
	this->_texture = Texture::GetIconImage(iconType, this->_alchemyItem);
}

void WheelItemAlchemy::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Slot;
		int itemCount = a_imap.contains(this->_alchemyItem) ? a_imap.find(this->_alchemyItem)->second.first : 0;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			fmt::format("{} ({})", _alchemyItem->GetName(), itemCount).data(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
		Drawer::draw_texture(_texture.texture,
			ImVec2(a_center.x, a_center.y),
			Config::Styling::Item::Slot::Texture::OffsetX,
			Config::Styling::Item::Slot::Texture::OffsetY,
			ImVec2(_texture.width * Config::Styling::Item::Slot::Texture::Scale, _texture.height * Config::Styling::Item::Slot::Texture::Scale),
			C_SKYRIMWHITE, a_drawArgs);
	}
}

void WheelItemAlchemy::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Highlight;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			_alchemyItem->GetName(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
	}

	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Highlight::Texture::OffsetX,
		Config::Styling::Item::Highlight::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Highlight::Texture::Scale, _texture.height * Config::Styling::Item::Highlight::Texture::Scale),
		C_SKYRIMWHITE, a_drawArgs);
}

bool WheelItemAlchemy::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

bool WheelItemAlchemy::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return a_inv.contains(this->_alchemyItem);
}

void WheelItemAlchemy::ActivateItemPrimary()
{
}

void WheelItemAlchemy::ActivateItemSecondary()
{
}

void WheelItemAlchemy::ActivateItemSpecial()
{
}

void WheelItemAlchemy::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = WheelItemAlchemy::ITEM_TYPE_STR;
	a_json["formID"] = this->_alchemyItem->GetFormID();
}
