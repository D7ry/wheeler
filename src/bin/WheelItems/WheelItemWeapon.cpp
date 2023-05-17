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

WheelItemWeapon::WheelItemWeapon(RE::TESObjectWEAP* a_weapon)
{
	_weapon = a_weapon;
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

std::pair<RE::TESBoundObject*, uint32_t> WheelItemWeapon::getInvBoundObject(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	for (auto& [item, count] : a_inv) {
		if (item->GetFormType() == RE::FormType::Weapon) {
			if (item->GetFormID() == _weapon->GetFormID()) {
				return { item, count.first };
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
	std::pair<RE::TESBoundObject*, uint32_t> invBoundObj = this->getInvBoundObject(inv); // obj, count
	if (invBoundObj.second < 2) { // we have less than 2, meaning we can't dual-wield
		int hand = pc->GetWeaponEquippedHand(_weapon);
		if ((hand == 1 && !a_toRight) || (hand == 0 && a_toRight)) {
			RE::ActorEquipManager::GetSingleton()->UnequipObject(pc, invBoundObj.first);
		}
	}
	
	auto slot = a_toRight ? Utils::Slot::GetRightHandSlot() : Utils::Slot::GetLeftHandSlot();
	RE::ActorEquipManager::GetSingleton()->EquipObject(pc, invBoundObj.first, nullptr, 1, slot);
}

bool WheelItemWeapon::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	return pc && pc->GetWeaponEquippedHand(_weapon) != -1;
}
bool WheelItemWeapon::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return this->getInvBoundObject(a_inv).first != nullptr;
}
