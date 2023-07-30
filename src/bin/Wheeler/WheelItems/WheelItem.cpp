#include "WheelItem.h"
#include "bin/Rendering/Drawer.h"
#include "bin/Config.h"
void WheelItem::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	//Drawer::draw_element(_texture, ImVec2(0, 0), ImVec2(100, 100), 0);
}

void WheelItem::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
}


bool WheelItem::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

bool WheelItem::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

void WheelItem::ActivateItemSecondary()
{
}

void WheelItem::ActivateItemPrimary()
{
}

void WheelItem::ActivateItemSpecial()
{
}

void WheelItem::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = ITEM_TYPE_STR;
	a_json["WARNING"] = "This is a placeholder item. It should not be used.";
}

void WheelItem::DrawItemHighlightStatIconAndValue(ImVec2 a_center, Texture::Image& a_stat_icon, float a_stat_value, DrawArgs a_drawArgs)
{
	Drawer::draw_text(
		a_center.x + Config::Styling::Item::Highlight::StatText::OffsetX,
		a_center.y + Config::Styling::Item::Highlight::StatText::OffsetY,
		fmt::format(": {}", int(std::ceil(a_stat_value))).data(),
		C_SKYRIMWHITE,
		Config::Styling::Item::Highlight::StatText::Size,
		a_drawArgs,
		false);
	
	Drawer::draw_texture(
		a_stat_icon.texture,
		a_center,
		Config::Styling::Item::Highlight::StatIcon::OffsetX,
		Config::Styling::Item::Highlight::StatIcon::OffsetY,
		ImVec2(a_stat_icon.width * Config::Styling::Item::Highlight::StatIcon::Scale, a_stat_icon.height * Config::Styling::Item::Highlight::StatIcon::Scale),
		C_SKYRIMWHITE,
		a_drawArgs);
}

