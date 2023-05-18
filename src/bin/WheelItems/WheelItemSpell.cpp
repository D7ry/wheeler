#include "WheelItemSpell.h"
#include "bin/Utils.h"
#include "include/lib/Drawer.h"
WheelItemSpeel::WheelItemSpeel(RE::SpellItem* a_spell)
{
	_spell = a_spell;
	const auto* effect = a_spell->GetCostliestEffectItem()->baseEffect;
	auto actorValue = effect->GetMagickSkill();
	if (actorValue == RE::ActorValue::kNone) {
		actorValue = effect->data.primaryAV;
	}
	Texture::icon_image_type iconType;
	switch (actorValue) {
	case RE::ActorValue::kAlteration:
		iconType = Texture::icon_image_type::alteration;
		break;
	case RE::ActorValue::kConjuration:
		iconType = Texture::icon_image_type::conjuration;
		break;
	case RE::ActorValue::kDestruction:
		switch (effect->data.resistVariable) {
		case RE::ActorValue::kResistFire:
			iconType = Texture::icon_image_type::destruction_fire;
			break;
		case RE::ActorValue::kResistFrost:
			iconType = Texture::icon_image_type::destruction_frost;
			break;
		case RE::ActorValue::kResistShock:
			iconType = Texture::icon_image_type::destruction_shock;
			break;
		default:
			iconType = Texture::icon_image_type::destruction;
		}
		break;
	case RE::ActorValue::kIllusion:
		iconType = Texture::icon_image_type::illusion;
		break;
	case RE::ActorValue::kRestoration:
		//might not fit all spells
		iconType = Texture::icon_image_type::restoration;
		break;
	default:
		iconType = Texture::icon_image_type::spell_default;
	}
	this->_texture = Texture::GetIconImage(iconType);
}

void WheelItemSpeel::DrawSlot(ImVec2 a_center, bool a_hovered)
{
	Drawer::draw_text(a_center.x, a_center.y,
		Config::Styling::Item::Slot::Text::OffsetX, Config::Styling::Item::Slot::Text::OffsetY,
		_spell->GetName(), 255, 255, 255, 255,
		Config::Styling::Item::Slot::Text::Size);
	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Slot::Texture::OffsetX,
		Config::Styling::Item::Slot::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Slot::Texture::Scale, _texture.height * Config::Styling::Item::Slot::Texture::Scale),
		0);
}

void WheelItemSpeel::DrawHighlight(ImVec2 a_center)
{
	Drawer::draw_text(a_center.x, a_center.y,
		Config::Styling::Item::Highlight::Text::OffsetX, Config::Styling::Item::Highlight::Text::OffsetY,
		_spell->GetName(), 255, 255, 255, 255,
		Config::Styling::Item::Highlight::Text::Size);
	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Highlight::Texture::OffsetX,
		Config::Styling::Item::Highlight::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Highlight::Texture::Scale, _texture.height * Config::Styling::Item::Highlight::Texture::Scale),
		0);
}

bool WheelItemSpeel::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return false;
	}
	auto lhs = pc->GetEquippedObject(true);
	bool lhsEquipped = lhs && lhs->GetFormID() == this->_spell->GetFormID();
	
	auto rhs = pc->GetEquippedObject(false);
	bool rhsEquipped = rhs && rhs->GetFormID() == this->_spell->GetFormID();
	
	
	return lhsEquipped || rhsEquipped;
}

bool WheelItemSpeel::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	return pc && pc->HasSpell(this->_spell);
}

void WheelItemSpeel::ActivateItemLeft()
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		RE::ActorEquipManager::GetSingleton()->EquipSpell(pc, this->_spell, Utils::Slot::GetLeftHandSlot());
	}
}

void WheelItemSpeel::ActivateItemRight()
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		RE::ActorEquipManager::GetSingleton()->EquipSpell(pc, this->_spell, Utils::Slot::GetRightHandSlot());
	}
}
