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
	if (iconType == Texture::icon_image_type::spell_default) { // haven't found spell icon yet, maybe a power
		if (a_spell->data.spellType == RE::MagicSystem::SpellType::kPower ||
			a_spell->data.spellType == RE::MagicSystem::SpellType::kLesserPower) {
			iconType = Texture::icon_image_type::power;
		}
	}
	this->_texture = Texture::GetIconImage(iconType, a_spell);
	RE::BSString descriptionBuf = "";
	this->_spell->GetDescription(descriptionBuf, nullptr);
	this->_description = descriptionBuf.c_str();
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
	
	std::string descriptionBuf = "";
	descriptionBuf = this->_description;
	
	if (descriptionBuf.empty()) { // get description of magic effect only if the original description is empty
		Utils::Magic::GetMagicItemDescription(_spell, descriptionBuf);
	}
	Drawer::draw_text_block(a_center.x + Config::Styling::Item::Highlight::Desc::OffsetX, a_center.y + Config::Styling::Item::Highlight::Desc::OffsetY,
		descriptionBuf, C_SKYRIMWHITE, Config::Styling::Item::Highlight::Desc::Size, Config::Styling::Item::Highlight::Desc::LineSpacing, Config::Styling::Item::Highlight::Desc::LineLength, a_drawArgs);

}

bool WheelItemSpell::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return false;
	}
	bool lhsEquipped = false;
	bool rhsEquipped = false;
	bool powerEquipped = false;
	
	if (this->isPower()) { // is power, meaning it can be equipped to shout slot
		auto power = pc->GetActorRuntimeData().selectedPower;
		if (power) {
			powerEquipped = power->GetFormID() == this->_spell->GetFormID();
		}
	}
	// power can also be equipped to lhs or rhs, so we're check them as well.
	auto lhs = pc->GetEquippedObject(true);
	lhsEquipped = lhs && lhs->GetFormID() == this->_spell->GetFormID();

	auto rhs = pc->GetEquippedObject(false);
	rhsEquipped = rhs && rhs->GetFormID() == this->_spell->GetFormID();

	return lhsEquipped || rhsEquipped || powerEquipped;
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
		if (this->isPower()) {
			if (pc->GetActorRuntimeData().selectedPower && pc->GetActorRuntimeData().selectedPower->GetFormID() == this->_spell->GetFormID()) {
				RE::ActorEquipManager::GetSingleton()->UnEquipSpell(pc, this->_spell, 2);
				RE::ActorEquipManager::GetSingleton()->UnEquipSpell(pc, this->_spell, 1);
				RE::ActorEquipManager::GetSingleton()->UnEquipSpell(pc, this->_spell, 0);
			} else {
				RE::ActorEquipManager::GetSingleton()->EquipSpell(pc, this->_spell, Utils::Slot::GetLeftHandSlot());
			}
		} else {
			if (pc->GetEquippedObject(true) && pc->GetEquippedObject(true)->GetFormID() == this->_spell->GetFormID()) {
				//Utils::Slot::CleanSlot(pc, Utils::Slot::GetLeftHandSlot());
				RE::ActorEquipManager::GetSingleton()->UnEquipSpell(pc, this->_spell, 0);
			} else {
				RE::ActorEquipManager::GetSingleton()->EquipSpell(pc, this->_spell, Utils::Slot::GetLeftHandSlot());
			}
		}
	}
}

void WheelItemSpell::ActivateItemPrimary()
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		// check if spell is already equiped, if it is, unequip.
		if (this->isPower()) {
			if (pc->GetActorRuntimeData().selectedPower && pc->GetActorRuntimeData().selectedPower->GetFormID() == this->_spell->GetFormID()) {
				RE::ActorEquipManager::GetSingleton()->UnEquipSpell(pc, this->_spell, 2);
				RE::ActorEquipManager::GetSingleton()->UnEquipSpell(pc, this->_spell, 1);
				RE::ActorEquipManager::GetSingleton()->UnEquipSpell(pc, this->_spell, 0);

			} else {
				RE::ActorEquipManager::GetSingleton()->EquipSpell(pc, this->_spell, Utils::Slot::GetRightHandSlot());
			}
		}
		else {
			if (pc->GetEquippedObject(false) && pc->GetEquippedObject(false)->GetFormID() == this->_spell->GetFormID()) {
				RE::ActorEquipManager::GetSingleton()->UnEquipSpell(pc, this->_spell, 1);
			} else {
				RE::ActorEquipManager::GetSingleton()->EquipSpell(pc, this->_spell, Utils::Slot::GetRightHandSlot());
			}
		}
	}
}


void WheelItemSpell::ActivateItemSpecial()
{
	return; // current don't do anything because I'm yet to figure out how to prevent the power from being casted when it shouldn't
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return;
	}
	if (this->_spell->data.flags.any(RE::SpellItem::SpellFlag::kInstantCast)
		|| this->_spell->GetSpellType() == RE::MagicSystem::SpellType::kPower 
		|| this->_spell->GetSpellType() == RE::MagicSystem::SpellType::kLesserPower) { // insta cast the spell or power
		
		auto selfTargeting = this->_spell->GetDelivery() == RE::MagicSystem::Delivery::kSelf;
		RE::TESObjectREFR* target = selfTargeting ? pc : pc->GetActorRuntimeData().currentCombatTarget.get().get();
		float castMagnitude = _spell->GetCostliestEffectItem() ? _spell->GetCostliestEffectItem()->GetMagnitude() : 1.f;
		
		RE::MagicCaster* caster = pc->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		float strength = 0;
		RE::MagicSystem::CannotCastReason reason = RE::MagicSystem::CannotCastReason::kOK;
		caster->CheckCast(this->_spell, false, &strength, &reason, false);
		if (reason == RE::MagicSystem::CannotCastReason::kOK) {
			caster->CastSpellImmediate(this->_spell, false, target, 1.f, false, castMagnitude, selfTargeting ? nullptr : pc);
		}
	} 
}

inline bool WheelItemSpell::isPower()
{
	auto spellType = this->_spell->GetSpellType();
	return spellType == RE::MagicSystem::SpellType::kAbility || spellType == RE::MagicSystem::SpellType::kPower || spellType == RE::MagicSystem::SpellType::kLesserPower;
}


void WheelItemSpell::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = WheelItemSpell::ITEM_TYPE_STR;
	a_json["formID"] = this->_spell->GetFormID();
}
