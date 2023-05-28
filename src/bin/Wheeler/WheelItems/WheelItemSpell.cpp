#include "bin/Utilities/Utils.h"
#include "bin/Rendering/Drawer.h"

#include "WheelItemSpell.h"
WheelItemSpell::WheelItemSpell(RE::SpellItem* a_spell)
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

void WheelItemSpell::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Slot;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			this->_spell->GetName(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
	}

	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Slot::Texture::OffsetX,
		Config::Styling::Item::Slot::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Slot::Texture::Scale, _texture.height * Config::Styling::Item::Slot::Texture::Scale),
		C_SKYRIMWHITE,
		a_drawArgs);
}

void WheelItemSpell::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	{
		using namespace Config::Styling::Item::Highlight;
		Drawer::draw_text(a_center.x + Text::OffsetX, a_center.y + Text::OffsetY,
			_spell->GetName(), C_SKYRIMWHITE, Text::Size, a_drawArgs);
	}
	
	Drawer::draw_texture(_texture.texture,
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Highlight::Texture::OffsetX,
		Config::Styling::Item::Highlight::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Highlight::Texture::Scale, _texture.height * Config::Styling::Item::Highlight::Texture::Scale),
		C_SKYRIMWHITE,
		a_drawArgs);
}

bool WheelItemSpell::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
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

bool WheelItemSpell::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	return pc && pc->HasSpell(this->_spell);
}

void WheelItemSpell::ActivateItemSecondary()
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		RE::ActorEquipManager::GetSingleton()->EquipSpell(pc, this->_spell, Utils::Slot::GetLeftHandSlot());
	}
}

void WheelItemSpell::ActivateItemPrimary()
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		RE::ActorEquipManager::GetSingleton()->EquipSpell(pc, this->_spell, Utils::Slot::GetRightHandSlot());
	}
}

void WheelItemSpell::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = WheelItemSpell::ITEM_TYPE_STR;
	a_json["formID"] = this->_spell->GetFormID();
}
