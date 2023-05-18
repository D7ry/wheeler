#include "WheelItemWeapon.h"
#include "include/lib/Drawer.h"
#include "bin/Utils.h"
void WheelItemWeapon::DrawSlot(ImVec2 a_center, bool a_hovered)
{
	Drawer::draw_text(a_center.x, a_center.y, 
		Config::Styling::Item::Slot::Text::OffsetX, Config::Styling::Item::Slot::Text::OffsetY,
		_weapon->GetName(), 255, 255, 255, 255,
		Config::Styling::Item::Slot::Text::Size);
	Drawer::draw_texture(_texture.texture, 
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Slot::Texture::OffsetX,
		Config::Styling::Item::Slot::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Slot::Texture::Scale, _texture.height * Config::Styling::Item::Slot::Texture::Scale), 
		0);
	//PieMenu::PieMenuItem("one weapon");
}

void WheelItemWeapon::DrawHighlight(ImVec2 a_center)
{
	Drawer::draw_text(a_center.x, a_center.y,
		Config::Styling::Item::Highlight::Text::OffsetX, Config::Styling::Item::Highlight::Text::OffsetY,
		_weapon->GetName(), 255, 255, 255, 255,
		Config::Styling::Item::Highlight::Text::Size);
	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Highlight::Texture::OffsetX,
		Config::Styling::Item::Highlight::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Highlight::Texture::Scale, _texture.height * Config::Styling::Item::Highlight::Texture::Scale),
		0);
}

WheelItemWeapon::WheelItemWeapon(RE::TESObjectWEAP* a_weapon, RE::EnchantmentItem* a_enchant, float a_health)
{
	_weapon = a_weapon;
	_enchant = a_enchant;
	_weaponHealth = a_health;
	// get weapon's texture
	// TODO: add support for animated armory/2h mace
	switch (a_weapon->GetWeaponType()) {
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
}

void WheelItemWeapon::ActivateItemLeft()
{
	equipItem(false);
}

void WheelItemWeapon::ActivateItemRight()
{
	equipItem(true);
}

std::pair<RE::TESBoundObject*, int> WheelItemWeapon::getInvBoundObject(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	//RE::TESObjectREFR::InventoryItemMap inv = RE::PlayerCharacter::GetSingleton()->GetInventory();
	for (auto& [boundObj, invEntry] : a_inv) {
		if (boundObj->GetFormType() == RE::FormType::Weapon) { // object type match
			if (boundObj->GetFormID() == _weapon->GetFormID()) {
				// todo: add proper enchantment filtering
				//std::pair<RE::EnchantmentItem*, float> extraDataPair = Utils::Inventory::GetEntryEnchantAndHealth(invEntry.second);
				//if (extraDataPair.first == this->_enchant && extraDataPair.second == this->_weaponHealth) {
					return { boundObj, invEntry.first };
				//}
			}
		}
	}
	return { nullptr, -1 };
}

void WheelItemWeapon::equipItem(bool a_toRight)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc || !pc->Is3DLoaded()) {
		return;
	}
	RE::TESObjectREFR::InventoryItemMap inv = pc->GetInventory();
	std::pair<RE::TESBoundObject*, int> invBoundObj = this->getInvBoundObject(inv);  // obj, count
	if (invBoundObj.first == nullptr) {                                                   // does not exist in inv
		return;
	}

	if (invBoundObj.second < 2) {  // we have less than 2, meaning we can't dual-wield
		RE::Actor::Hand hand = pc->GetWeaponEquippedHand(_weapon);
		if ((hand == RE::Actor::Hand::Right && !a_toRight) || (hand == RE::Actor::Hand::Right && a_toRight)) {
			auto oppositeSlot = a_toRight ? Utils::Slot::GetLeftHandSlot() : Utils::Slot::GetRightHandSlot();
			//RE::ActorEquipManager::GetSingleton()->UnequipObject(pc, invBoundObj.first, nullptr, 1, oppositeSlot);
			Utils::Slot::CleanSlot(pc, oppositeSlot);
		}
	}
	if (_weapon->IsTwoHanded()) {  // clean up both slots
		Utils::Slot::CleanSlot(pc, Utils::Slot::GetLeftHandSlot());
		Utils::Slot::CleanSlot(pc, Utils::Slot::GetRightHandSlot());
	}
	auto slot = a_toRight ? Utils::Slot::GetRightHandSlot() : Utils::Slot::GetLeftHandSlot();
	RE::ActorEquipManager::GetSingleton()->EquipObject(pc, invBoundObj.first, nullptr, 1, slot);
	
	
}

bool WheelItemWeapon::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	//RE::TESObjectREFR::InventoryItemMap inv = pc->GetInventory();
	auto invData = this->getInvBoundObject(a_inv);
	if (invData.first == nullptr) {
		return false;
	}
	return pc && pc->GetWeaponEquippedHand(invData.first) != RE::Actor::Hand::None;
}
bool WheelItemWeapon::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();

	//RE::TESObjectREFR::InventoryItemMap inv = pc->GetInventory();

	return this->getInvBoundObject(a_inv).first != nullptr;
}
