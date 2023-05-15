#pragma once
#include "WheelItem.h"

class WheelItemWeapon : public WheelItem
{
public:
	void Draw(ImVec2 a_center, bool a_hovered) override;
	
	WheelItemWeapon(RE::TESObjectWEAP* a_weapon) 
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


private:
	RE::TESObjectWEAP* _weapon;
};
