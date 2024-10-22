#include "bin/Rendering/Drawer.h"
#include "bin/Utilities/Utils.h"
#include "bin/Wheeler/Wheeler.h"
#include "bin/Texts.h"
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
		case RE::ActorValue::kStaminaRate:
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
	//显示物品名称和数量
	//之前崩溃都是因为在使用了自制药水后尤其是回血回蓝的药水， itemName 会指向一个很奇怪的地址，
	//基本上是一个很小的非法地址例如0x0000006a这种为所以空导致接下来fmt::format()函数崩溃
	//不知道为什么会指向这个地址，其他药水都不会指向这么小的地址
	//可能又是CE引擎的怪癖，所以这里再加了一个判断，如果地址小于0x10000就直接显示一个默认名称
	//The crash was caused by the itemName pointing to a very strange address after using self-made potions,
	// especially potions that restore health and magicka. The address is basically a very small illegal address such as 0x0000006a, 
	// so the fmt::format() function crashes next. 
	// I don't know why it points to this address, and other self-made potions don't point to such a small address. 
	// It may be another quirk of the CE engine, so I added another judgment here. 
	// If the address is less than 0x10000, a default name is displayed directly.
	//第二道检查，避免崩溃
	if (this->_alchemyItem) {
		//提前检查 _alchemyItem 是否有名称，并避免崩溃
		const auto fullName = this->_alchemyItem->As<RE::TESFullName>();
		const char* itemName = nullptr;

		if (fullName) {
			itemName = fullName->GetFullName();
		}

		if (!itemName || reinterpret_cast<std::uintptr_t>(itemName) < 0x10000 || *itemName == '\0') {
			itemName = "Consumed out Item";  //设置一个默认名称，避免非法内存访问，现在一般正常游戏内永远不会看到这个名称
		}

		//获取物品数量
		int itemCount = a_imap.contains(this->_alchemyItem) ? a_imap.find(this->_alchemyItem)->second.first : 0;


		std::string text = fmt::format("{} ({})", itemName, itemCount);
		this->drawSlotText(a_center, text.data(), a_drawArgs);
	} else {
		//如果 _alchemyItem 为空，显示占位符
		std::string text = "No Item";
		this->drawSlotText(a_center, text.data(), a_drawArgs);
	}
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
	///return a_inv.contains(this->_alchemyItem);
	//检查物品是否存在于玩家的库存中
	auto it = a_inv.find(this->_alchemyItem);
	if (it != a_inv.end()) {
		//检查物品的数量是否大于 0
		return it->second.first > 0;
	}
	return false;
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
	
	/*修复了自制药剂消耗后CTD的问题，所以也不需要检查数量了
	if (this->_alchemyItem->IsDynamicForm() && pc->GetItemCount(this->_alchemyItem) <= 1) {
		Utils::NotificationMessage(Texts::GetText(Texts::TextType::AlchemyDynamicIDConsumptionWarning));
		return;
	}*/
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
