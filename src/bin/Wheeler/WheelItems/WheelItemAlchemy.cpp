#include "bin/Rendering/Drawer.h"
#include "bin/Utilities/Utils.h"
#include "bin/Wheeler/Wheeler.h"
#include "bin/Messages.h"
#include "WheelItemAlchemy.h"

WheelItemAlchemy::WheelItemAlchemy(RE::AlchemyItem* a_alchemyItem)
{
	this->_alchemyItem = a_alchemyItem;
	Texture::icon_image_type iconType = Texture::icon_image_type::food;
	if (_alchemyItem->data.flags.any(RE::AlchemyItem::AlchemyFlag::kFoodItem)  // food icon
		|| _alchemyItem->HasKeywordString("VendorItemFood")) {
		iconType = Texture::icon_image_type::food;
		this->_alchemyItemType = WheelItemAlchemyType::kFood;
	} else if (_alchemyItem->data.flags.any(RE::AlchemyItem::AlchemyFlag::kPoison)  // poison icon
			   || _alchemyItem->HasKeywordString("VendorItemPoison")) {
		iconType = Texture::icon_image_type::poison_default;
		this->_alchemyItemType = WheelItemAlchemyType::kPoison;
	} else if (_alchemyItem->data.flags.any(RE::AlchemyItem::AlchemyFlag::kMedicine)  //potion icon
			   || _alchemyItem->HasKeywordString("VendorItemPotion")) {
		// https://github.com/mlthelama/LamasTinyHUD/blob/main/src/handle/page_handle.cpp
		iconType = Texture::icon_image_type::potion_default;
		this->_alchemyItemType = WheelItemAlchemyType::kPotion;
		const RE::EffectSetting* effect = _alchemyItem->GetCostliestEffectItem()->baseEffect;
		RE::ActorValue actorValue = effect->GetMagickSkill();
		if (actorValue == RE::ActorValue::kNone) {
			actorValue = effect->data.primaryAV;
		}
		switch (actorValue) {
		case RE::ActorValue::kHealth:
		case RE::ActorValue::kHealRateMult:
		case RE::ActorValue::kHealRate:
			iconType = Texture::icon_image_type::potion_health;
			break;
		case RE::ActorValue::kStamina:
		case RE::ActorValue::kStaminaRateMult:
		case RE::ActorValue::KStaminaRate:
			iconType = Texture::icon_image_type::potion_stamina;
			break;
		case RE::ActorValue::kMagicka:
		case RE::ActorValue::kMagickaRateMult:
		case RE::ActorValue::kMagickaRate:
			iconType = Texture::icon_image_type::potion_magicka;
			break;
		case RE::ActorValue::kResistFire:
			iconType = Texture::icon_image_type::potion_fire_resist;
			break;
		case RE::ActorValue::kResistShock:
			iconType = Texture::icon_image_type::potion_shock_resist;
			break;
		case RE::ActorValue::kResistFrost:
			iconType = Texture::icon_image_type::potion_frost_resist;
			break;
		case RE::ActorValue::kResistMagic:
			iconType = Texture::icon_image_type::potion_magic_resist;
			break;
		default:
			iconType = Texture::icon_image_type::potion_default;
		}
	}
	this->_texture = Texture::GetIconImage(iconType, this->_alchemyItem);
	Utils::Magic::GetMagicItemDescription(_alchemyItem, this->_description);
}

void WheelItemAlchemy::DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{
	int itemCount = a_imap.contains(this->_alchemyItem) ? a_imap.find(this->_alchemyItem)->second.first : 0;
	std::string text = fmt::format("{} ({})", _alchemyItem->GetName(), itemCount);
	this->drawSlotText(a_center, text.data(), a_drawArgs);
	this->drawSlotTexture(a_center, a_drawArgs);
}

void WheelItemAlchemy::DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs)
{	
	this->drawHighlightText(a_center, _alchemyItem->GetName(), a_drawArgs);
	this->drawHighlightTexture(a_center, a_drawArgs);
	if (!this->_description.empty()) {
		this->drawHighlightDescription(a_center, this->_description.data(), a_drawArgs);
	}
}

bool WheelItemAlchemy::IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return false;
}

bool WheelItemAlchemy::IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	return a_inv.contains(this->_alchemyItem);
}

void WheelItemAlchemy::ActivateItemPrimary()
{
	switch (this->_alchemyItemType) {
	case WheelItemAlchemyType::kPotion:
	case WheelItemAlchemyType::kFood:
		this->consume();
		break;
	case WheelItemAlchemyType::kPoison:
		this->applyPoison();
		break;
	}
}

void WheelItemAlchemy::ActivateItemSecondary()
{
	switch (this->_alchemyItemType) {
	case WheelItemAlchemyType::kPotion:
	case WheelItemAlchemyType::kFood:
		this->consume();
		break;
	case WheelItemAlchemyType::kPoison:
		this->applyPoison();
		break;
	}
}

void WheelItemAlchemy::ActivateItemSpecial()
{
	return;
}

void WheelItemAlchemy::SerializeIntoJsonObj(nlohmann::json& a_json)
{
	a_json["type"] = WheelItemAlchemy::ITEM_TYPE_STR;
	a_json["formID"] = this->_alchemyItem->GetFormID();
}

void WheelItemAlchemy::consume()
{
	if (this->_alchemyItemType != WheelItemAlchemyType::kFood && this->_alchemyItemType != WheelItemAlchemyType::kPotion) {
		throw std::exception("WheelItemAlchemy::consume : Item is not a food or potion");
	}
	RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return;
	}
	if (this->_alchemyItem->IsDynamicForm() && pc->GetInventoryItemCount(this->_alchemyItem) <= 1) {
		return;
	}
	RE::ActorEquipManager::GetSingleton()->EquipObject(pc, this->_alchemyItem);
}

void WheelItemAlchemy::applyPoison()
{
	if (this->_alchemyItemType != WheelItemAlchemyType::kPoison) {
		throw std::exception("WheelItemAlchemy::applyPoison : Item is not a poison");
	}
	RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return;
	}
	RE::ActorEquipManager::GetSingleton()->EquipObject(pc, this->_alchemyItem);
	Wheeler::TryCloseWheeler(); // close wheeler for the pop-up
}
