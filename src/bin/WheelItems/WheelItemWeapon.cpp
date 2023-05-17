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
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc || !pc->Is3DLoaded()) {
		return;
	}
	auto inv = pc->GetInventory();
	if (!this->IsAvailable(inv)) {
		return;
	}
	RE::ActorEquipManager::GetSingleton()->EquipObject(pc, this->getBoundObject(inv), nullptr, 1, Utils::Slot::GetLeftHandSlot());
}

void WheelItemWeapon::ActivateItemRight()
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc || !pc->Is3DLoaded()) {
		return;
	}
	auto inv = pc->GetInventory();
	if (!this->IsAvailable(inv)) {
		return;
	}
	RE::ActorEquipManager::GetSingleton()->EquipObject(pc, this->getBoundObject(inv), nullptr, 1, Utils::Slot::GetRightHandSlot());
}

RE::TESBoundObject* WheelItemWeapon::getBoundObject(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	for (auto& [item, count] : a_inv) {
		if (item->GetFormType() == RE::FormType::Weapon) {
			if (item->GetFormID() == _weapon->GetFormID()) {
				return item;
			}
		}
	}
	return nullptr;
}

bool WheelItemWeapon::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	return pc && pc->IsWeaponEquipped(_weapon);
}
bool WheelItemWeapon::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return this->getBoundObject(a_inv) != nullptr;
}
