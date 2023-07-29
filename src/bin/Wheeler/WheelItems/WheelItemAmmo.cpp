#include "WheelItemAmmo.h"
#include "bin/Rendering/Drawer.h"
#include "bin/Rendering/TextureManager.h"

WheelItemAmmo::WheelItemAmmo(RE::TESAmmo* a_ammo)
{
	this->_ammo = a_ammo;
	// load texture
	this->_texture = Texture::GetIconImage(Texture::icon_image_type::arrow, a_ammo);
}

void WheelItemAmmo::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Slot;
		int ammoCount = a_imap.contains(this->_ammo) ? a_imap.find(this->_ammo)->second.first : 0;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			fmt::format("{} ({})", _ammo->GetName(), ammoCount).data(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
		Drawer::draw_texture(_texture.texture,
			ImVec2(a_center.x, a_center.y),
			Config::Styling::Item::Slot::Texture::OffsetX,
			Config::Styling::Item::Slot::Texture::OffsetY,
			ImVec2(_texture.width * Config::Styling::Item::Slot::Texture::Scale, _texture.height * Config::Styling::Item::Slot::Texture::Scale),
			C_SKYRIMWHITE, a_drawArgs);
	}
}

void WheelItemAmmo::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Highlight;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			_ammo->GetName(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
	}

	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Highlight::Texture::OffsetX,
		Config::Styling::Item::Highlight::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Highlight::Texture::Scale, _texture.height * Config::Styling::Item::Highlight::Texture::Scale),
		C_SKYRIMWHITE, a_drawArgs);
}

bool WheelItemAmmo::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	if (!a_inv.contains(this->_ammo)) {
		return false;
	}
	auto entry = a_inv.find(this->_ammo)->second.second.get();
	return entry && entry->IsWorn();
}

bool WheelItemAmmo::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return a_inv.contains(this->_ammo);
}

void WheelItemAmmo::ActivateItemSecondary()
{
	toggleEquip();
}

void WheelItemAmmo::ActivateItemPrimary()
{
	toggleEquip();
}

void WheelItemAmmo::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = WheelItemAmmo::ITEM_TYPE_STR;
	a_json["formID"] = this->_ammo->GetFormID();
}

void WheelItemAmmo::toggleEquip()
{
	RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();
	RE::ActorEquipManager* aeMan = RE::ActorEquipManager::GetSingleton();
	if (!pc || !aeMan) {
		return;
	}
	RE::TESObjectREFR::InventoryItemMap invMap = pc->GetInventory();
	if (this->IsAvailable(invMap)) {
		if (this->IsActive(invMap)) {
			aeMan->UnequipObject(pc, this->_ammo);
		} else {
			aeMan->EquipObject(pc, this->_ammo);
		}
	}
}
