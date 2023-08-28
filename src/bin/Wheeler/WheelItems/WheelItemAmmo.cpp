#include "WheelItemAmmo.h"
#include "bin/Rendering/Drawer.h"
#include "bin/Rendering/TextureManager.h"

WheelItemAmmo::WheelItemAmmo(RE::TESAmmo* a_ammo)
{
	this->_ammo = a_ammo;
	// load texture
	this->_texture = Texture::GetIconImage(Texture::icon_image_type::arrow, a_ammo);
	this->_stat_texture = Texture::GetIconImage(Texture::icon_image_type::weapon_damage, nullptr);
	RE::BSString descriptionBuf = "";
	a_ammo->GetDescription(descriptionBuf, nullptr);
	this->_description = std::string(descriptionBuf.c_str());
}

void WheelItemAmmo::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	int ammoCount = a_imap.contains(this->_ammo) ? a_imap.find(this->_ammo)->second.first : 0;
	std::string text = fmt::format("{} ({})", _ammo->GetName(), ammoCount);
	this->drawSlotText(a_center, text.data(), a_drawArgs);
	this->drawSlotTexture(a_center, a_drawArgs);
}

void WheelItemAmmo::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	this->drawHighlightText(a_center, _ammo->GetName(), a_drawArgs);
	this->drawHighlightTexture(a_center, a_drawArgs);
	if (!this->_description.empty()) {
		this->drawHighlightDescription(a_center, this->_description.data(), a_drawArgs);
	}
	float ammoDamage = 0;

	if (a_imap.contains(this->_ammo)) {
		RE::InventoryEntryData* inventoryEntry = a_imap.find(this->_ammo)->second.second.get();
		ammoDamage = RE::PlayerCharacter::GetSingleton()->GetDamage(inventoryEntry);
	} else {
		ammoDamage = this->_ammo->data.damage;
	}
	
	drawItemHighlightStatIconAndValue(a_center, this->_stat_texture, ammoDamage, a_drawArgs);
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


RE::TESForm* WheelItemAmmo::GetItemForm()
{
	return this->_ammo;
}
