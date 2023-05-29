#include "WheelItemWeapon.h"
#include "bin/Rendering/Drawer.h"
#include "bin/Utilities/Utils.h"

void WheelItemWeapon::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	std::string text = this->_obj->GetName();
	int itemCount = this->GetItemData(a_imap).first;
	if (itemCount > 1) {
		text += " (" + std::to_string(itemCount) + ")";
	} 
	{
		using namespace Config::Styling::Item::Slot;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			text.data(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
		Drawer::draw_texture(_texture.texture,
			ImVec2(a_center.x, a_center.y),
			Config::Styling::Item::Slot::Texture::OffsetX,
			Config::Styling::Item::Slot::Texture::OffsetY,
			ImVec2(_texture.width * Config::Styling::Item::Slot::Texture::Scale, _texture.height * Config::Styling::Item::Slot::Texture::Scale),
			C_SKYRIMWHITE, a_drawArgs);
	}




	//PieMenu::PieMenuItem("one weapon");
}

void WheelItemWeapon::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Highlight;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			this->_obj->GetName(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
	}
	
	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Highlight::Texture::OffsetX,
		Config::Styling::Item::Highlight::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Highlight::Texture::Scale, _texture.height * Config::Styling::Item::Highlight::Texture::Scale),
		C_SKYRIMWHITE, a_drawArgs);

}

WheelItemWeapon::WheelItemWeapon(RE::TESBoundObject* a_weapon, uint16_t a_uniqueID)
{
	this->_obj = a_weapon;
	this->SetUniqueID(a_uniqueID);
	// get weapon's texture
	// TODO: add support for animated armory/2h mace
	switch (a_weapon->As<RE::TESObjectWEAP>()->GetWeaponType()) {
	case RE::WEAPON_TYPE::kBow:
		_texture = Texture::GetIconImage(Texture::icon_image_type::bow);
		break;
	case RE::WEAPON_TYPE::kCrossbow:
		_texture = Texture::GetIconImage(Texture::icon_image_type::crossbow);
		break;
	case RE::WEAPON_TYPE::kStaff:
		_texture = Texture::GetIconImage(Texture::icon_image_type::staff);
		break;
	case RE::WEAPON_TYPE::kHandToHandMelee:
		_texture = Texture::GetIconImage(Texture::icon_image_type::hand_to_hand);
		break;
	case RE::WEAPON_TYPE::kOneHandSword:
		_texture = Texture::GetIconImage(Texture::icon_image_type::sword_one_handed);
		break;
	case RE::WEAPON_TYPE::kOneHandDagger:
		_texture = Texture::GetIconImage(Texture::icon_image_type::dagger);
		break;
	case RE::WEAPON_TYPE::kOneHandAxe:
		_texture = Texture::GetIconImage(Texture::icon_image_type::axe_one_handed);
		break;
	case RE::WEAPON_TYPE::kOneHandMace:
		_texture = Texture::GetIconImage(Texture::icon_image_type::mace);
		break;
	case RE::WEAPON_TYPE::kTwoHandSword:
		_texture = Texture::GetIconImage(Texture::icon_image_type::sword_two_handed);
		break;
	case RE::WEAPON_TYPE::kTwoHandAxe:
		_texture = Texture::GetIconImage(Texture::icon_image_type::axe_two_handed);
		break;
	}
	RE::BSString descriptionBuf = "";
	a_weapon->As<RE::TESObjectWEAP>()->GetDescription(descriptionBuf, nullptr);
	this->_description = descriptionBuf.c_str();
}

void WheelItemWeapon::ActivateItemSecondary()
{
	equipItem(false);
}

void WheelItemWeapon::ActivateItemPrimary()
{
	equipItem(true);
}

void WheelItemWeapon::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = WheelItemWeapon::ITEM_TYPE_STR;
	a_json["formID"] = this->_obj->GetFormID();
	a_json["uniqueID"] = this->GetUniqueID();
}



void WheelItemWeapon::equipItem(bool a_toRight)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc || !pc->Is3DLoaded()) {
		return;
	}
	RE::TESObjectREFR::InventoryItemMap inv = pc->GetInventory();
	auto itemData = this->GetItemData(inv);
	int count = itemData.first;
	RE::ExtraDataList* extraData = itemData.second;
	if (count <= 0) { // nothing to equip
		return;
	}
	
	if (count < 2) {  // we have less than 2, meaning we can't dual-wield
		Utils::Inventory::Hand hand = Utils::Inventory::GetWeaponEquippedHand(pc, this->_obj->As<RE::TESObjectWEAP>(), this->GetUniqueID());
		if ((hand == Utils::Inventory::Hand::Right && !a_toRight) || (hand == Utils::Inventory::Hand::Left && a_toRight)) {
			auto oppositeSlot = a_toRight ? Utils::Slot::GetLeftHandSlot() : Utils::Slot::GetRightHandSlot();
			//RE::ActorEquipManager::GetSingleton()->UnequipObject(pc, invBoundObj.first, nullptr, 1, oppositeSlot);
			Utils::Slot::CleanSlot(pc, oppositeSlot);
		}
	} else { // a count bigger than 1 guarantees that the item is untempered
		extraData = nullptr; // set extraData to nullptr, let the game handle which one to equip.
	}
	if (this->_obj->As<RE::TESObjectWEAP>()->IsTwoHanded()) {  // clean up both slots
		Utils::Slot::CleanSlot(pc, Utils::Slot::GetLeftHandSlot());
		Utils::Slot::CleanSlot(pc, Utils::Slot::GetRightHandSlot());
	}
	auto slot = a_toRight ? Utils::Slot::GetRightHandSlot() : Utils::Slot::GetLeftHandSlot();
	RE::ActorEquipManager::GetSingleton()->EquipObject(pc, _obj, extraData, 1, slot);
	
}

bool WheelItemWeapon::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return false;
	}
	if (this->GetItemData(a_inv).first >= 2) {
		return Utils::Inventory::GetWeaponEquippedHand(pc, this->_obj->As<RE::TESObjectWEAP>(), this->GetUniqueID(), true) != Utils::Inventory::Hand::None;
	} else {
		return Utils::Inventory::GetWeaponEquippedHand(pc, this->_obj->As<RE::TESObjectWEAP>(), this->GetUniqueID()) != Utils::Inventory::Hand::None;
	}
}
bool WheelItemWeapon::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();

	auto itemData = this->GetItemData(a_inv);

	return itemData.first > 0;
}
