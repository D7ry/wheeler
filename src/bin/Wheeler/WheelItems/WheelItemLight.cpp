#include "WheelItemLight.h"
#include "bin/Rendering/TextureManager.h"
#include "bin/Rendering/Drawer.h"

WheelItemLight::WheelItemLight(RE::TESObjectLIGH* a_light)
{
	if (!a_light->CanBeCarried()) {
		throw std::invalid_argument("WheelItemLight::ctor: Light must be carryable");
	}
	this->_light = a_light;
	// load texture
	this->_texture = Texture::GetIconImage(Texture::icon_image_type::torch);
}

void WheelItemLight::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Slot;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			_light->GetName(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
		Drawer::draw_texture(_texture.texture,
			ImVec2(a_center.x, a_center.y),
			Config::Styling::Item::Slot::Texture::OffsetX,
			Config::Styling::Item::Slot::Texture::OffsetY,
			ImVec2(_texture.width * Config::Styling::Item::Slot::Texture::Scale, _texture.height * Config::Styling::Item::Slot::Texture::Scale),
			C_SKYRIMWHITE, a_drawArgs);
	}
}

void WheelItemLight::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Highlight;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			_light->GetName(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
	}

	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Highlight::Texture::OffsetX,
		Config::Styling::Item::Highlight::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Highlight::Texture::Scale, _texture.height * Config::Styling::Item::Highlight::Texture::Scale),
		C_SKYRIMWHITE, a_drawArgs);
}

bool WheelItemLight::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	if (!a_inv.contains(this->_light)) {
		return false;
	}
	auto entry = a_inv.find(this->_light)->second.second.get();
	return entry && entry->IsWorn();
}

bool WheelItemLight::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return a_inv.contains(this->_light);
}

void WheelItemLight::ActivateItemSecondary()
{
	toggleEquip();
}

void WheelItemLight::ActivateItemPrimary()
{
	toggleEquip();
}

void WheelItemLight::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = WheelItemLight::ITEM_TYPE_STR;
	a_json["formID"] = this->_light->GetFormID();
}


void WheelItemLight::toggleEquip()
{
	RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();
	RE::ActorEquipManager* aeMan = RE::ActorEquipManager::GetSingleton();
	if (!pc || !aeMan) {
		return;
	}
	RE::TESObjectREFR::InventoryItemMap invMap = pc->GetInventory();
	if (this->IsAvailable(invMap)) {
		if (this->IsActive(invMap)) {
			aeMan->UnequipObject(pc, this->_light);
		} else {
			aeMan->EquipObject(pc, this->_light);
		}
	}
}
