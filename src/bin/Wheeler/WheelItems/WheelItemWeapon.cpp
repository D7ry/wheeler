#include "WheelItemWeapon.h"
#include "bin/Rendering/Drawer.h"
#include "bin/Utilities/Utils.h"

void WheelItemWeapon::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	std::string text = this->_obj->GetName();
	int itemCount = this->GetItemExtraDataAndCount(a_imap).first;
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
	
	std::string descriptionBuf = "";
	// first check if description is empty, if not we just show the description itself, weapon's sepcial description is probably more important.
	if (!this->_description.empty()) {  // non-empty description, weapon's main description takes priority(it's probably a special weapon)
		descriptionBuf = this->_description;
	} else {
		// try to get enchant of this weapon
		std::vector<RE::EnchantmentItem*> enchants;
		this->GetItemEnchantment(a_imap, enchants);
		if (!enchants.empty()) {
			// take 1st item for now.
			Utils::Magic::GetMagicItemDescription(enchants[0], descriptionBuf);
		}
	}
	Drawer::draw_text_block(a_center.x + Config::Styling::Item::Highlight::Desc::OffsetX, a_center.y + Config::Styling::Item::Highlight::Desc::OffsetY,
		descriptionBuf, C_SKYRIMWHITE, Config::Styling::Item::Highlight::Desc::Size, Config::Styling::Item::Highlight::Desc::LineSpacing, Config::Styling::Item::Highlight::Desc::LineLength, a_drawArgs);

	int weaponDamage = this->_obj->As<RE::TESObjectWEAP>()->GetAttackDamage();
	drawItemHighlightStatIconAndValue(a_center, this->_stat_texture, weaponDamage, a_drawArgs);
}

WheelItemWeapon::WheelItemWeapon(RE::TESBoundObject* a_weapon, uint16_t a_uniqueID)
{
	this->_obj = a_weapon;
	this->SetUniqueID(a_uniqueID);
	// get weapon's texture
	// TODO: add support for animated armory/2h mace
	Texture::icon_image_type iconType = Texture::icon_image_type::sword_one_handed;
	switch (a_weapon->As<RE::TESObjectWEAP>()->GetWeaponType()) {
	case RE::WEAPON_TYPE::kBow:
		iconType = Texture::icon_image_type::bow;
		break;
	case RE::WEAPON_TYPE::kCrossbow:
		iconType = Texture::icon_image_type::crossbow;
		break;
	case RE::WEAPON_TYPE::kStaff:
		iconType = Texture::icon_image_type::staff;
		break;
	case RE::WEAPON_TYPE::kHandToHandMelee:
		iconType = Texture::icon_image_type::hand_to_hand;
		break;
	case RE::WEAPON_TYPE::kOneHandSword:
		iconType = Texture::icon_image_type::sword_one_handed;
		break;
	case RE::WEAPON_TYPE::kOneHandDagger:
		iconType = Texture::icon_image_type::dagger;
		break;
	case RE::WEAPON_TYPE::kOneHandAxe:
		iconType = Texture::icon_image_type::axe_one_handed;
		break;
	case RE::WEAPON_TYPE::kOneHandMace:
		iconType = Texture::icon_image_type::mace;
		break;
	case RE::WEAPON_TYPE::kTwoHandSword:
		iconType = Texture::icon_image_type::sword_two_handed;
		break;
	case RE::WEAPON_TYPE::kTwoHandAxe:
	{
		if (a_weapon->As<RE::TESObjectWEAP>()->HasKeywordString("WeapTypeWarhammer")) {
			iconType = Texture::icon_image_type::warhammer_two_handed;
		} else {
			iconType = Texture::icon_image_type::axe_two_handed;
		}
	}
		break;
	}
	_texture = Texture::GetIconImage(iconType, a_weapon);
	_stat_texture = Texture::GetIconImage(Texture::icon_image_type::weapon_damage, nullptr);

	RE::BSString descriptionBuf = "";
	a_weapon->As<RE::TESObjectWEAP>()->GetDescription(descriptionBuf, nullptr);
	this->_description = descriptionBuf.c_str();
}

void WheelItemWeapon::ActivateItemSecondary()
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return;
	}
	bool isTwoHanded = (this->_obj->As<RE::TESObjectWEAP>()->IsCrossbow() 
		|| this->_obj->As<RE::TESObjectWEAP>()->IsBow() 
		|| this->_obj->As<RE::TESObjectWEAP>()->IsTwoHanded());
	
	Utils::Inventory::Hand equippedHand = Utils::Inventory::GetWeaponEquippedHand(pc, this->_obj->As<RE::TESObjectWEAP>(), this->GetUniqueID(), true);
	if (equippedHand == Utils::Inventory::Hand::Left || (isTwoHanded && equippedHand == Utils::Inventory::Hand::Right)) {
		if (isTwoHanded) {
			unequipItem(Utils::Slot::GetRightHandSlot());  // note: 2 handed weapons need to be unequipped from the right hand slot to be truly unequipped.
		} else {
			unequipItem(Utils::Slot::GetLeftHandSlot());
		}
	} else {
		equipItem(false);
	}
}

void WheelItemWeapon::ActivateItemPrimary()
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return;
	}
	if (Utils::Inventory::GetWeaponEquippedHand(pc, this->_obj->As<RE::TESObjectWEAP>(), this->GetUniqueID(), true) == Utils::Inventory::Hand::Right) {
		unequipItem(Utils::Slot::GetRightHandSlot());
	} else {
		equipItem(true);
	}
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
	auto itemData = this->GetItemExtraDataAndCount(inv);
	int count = itemData.first;
	RE::ExtraDataList* extraData = itemData.second;
	if (count <= 0) { // nothing to equip
		return;
	}
	
	if (count < 2) {  // we have less than 2, meaning we can't dual-wield
		Utils::Inventory::Hand hand = Utils::Inventory::GetWeaponEquippedHand(pc, this->_obj->As<RE::TESObjectWEAP>(), this->GetUniqueID());
		if ((hand == Utils::Inventory::Hand::Right && !a_toRight) || (hand == Utils::Inventory::Hand::Left && a_toRight)) { // in opposite hands, simply swap l/r
			auto oppositeSlot = a_toRight ? Utils::Slot::GetLeftHandSlot() : Utils::Slot::GetRightHandSlot(); // first, clean the slot with item
			RE::ActorEquipManager::GetSingleton()->UnequipObject(pc, this->_obj, nullptr, 1, oppositeSlot, false, true, true);
		}
	} else { // a count bigger than 1 guarantees that the item is untempered
		extraData = nullptr; // set extraData to nullptr, let the game handle which one to equip.
	}
	if (this->_obj->As<RE::TESObjectWEAP>()->IsCrossbow() || this->_obj->As<RE::TESObjectWEAP>()->IsBow()) {  // clean up both slots
		Utils::Slot::CleanSlot(pc, Utils::Slot::GetLeftHandSlot());
		Utils::Slot::CleanSlot(pc, Utils::Slot::GetRightHandSlot());
	}
	auto slot = a_toRight ? Utils::Slot::GetRightHandSlot() : Utils::Slot::GetLeftHandSlot();
	RE::ActorEquipManager::GetSingleton()->EquipObject(pc, _obj, extraData, 1, slot);
	
}

void WheelItemWeapon::unequipItem(const RE::BGSEquipSlot* a_slot)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return;
	}
	auto aeMan = RE::ActorEquipManager::GetSingleton();
	if (!aeMan) {
		return;
	}
	aeMan->UnequipObject(pc, this->_obj, nullptr, 1, a_slot);
}

bool WheelItemWeapon::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return false;
	}
	if (this->GetItemExtraDataAndCount(a_inv).first >= 2) {
		return Utils::Inventory::GetWeaponEquippedHand(pc, this->_obj->As<RE::TESObjectWEAP>(), this->GetUniqueID(), true) != Utils::Inventory::Hand::None;
	} else {
		return Utils::Inventory::GetWeaponEquippedHand(pc, this->_obj->As<RE::TESObjectWEAP>(), this->GetUniqueID()) != Utils::Inventory::Hand::None;
	}
}
bool WheelItemWeapon::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();

	auto itemData = this->GetItemExtraDataAndCount(a_inv);

	return itemData.first > 0;
}
