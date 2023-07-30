#include "nlohmann/json.hpp"

#include "WheelItemFactory.h"
#include "bin/Utilities/Utils.h"
#include "WheelItem.h"
#include "WheelItemSpell.h"
#include "WheelItemWeapon.h"
#include "WheelItemArmor.h"
#include "WheelItemShout.h"
#include "WheelItemLight.h"
#include "WheelItemAmmo.h"
#include "WheelItemAlchemy.h"

std::shared_ptr<WheelItem> WheelItemFactory::MakeWheelItemFromMenuHovered()
{
	RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();
	if (!pc || !pc->Is3DLoaded()) {
		return nullptr;
	}
	RE::UI* ui = RE::UI::GetSingleton();
	if (!ui) {
		return nullptr;
	}
	try
	{
		if (ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME)) {
			auto* invMenu = static_cast<RE::InventoryMenu*>(ui->GetMenu(RE::InventoryMenu::MENU_NAME).get());
			if (!invMenu) {
				return nullptr;
			}
			RE::InventoryEntryData* invEntry = Utils::Inventory::GetSelectedItemIninventory(invMenu);
			if (!invEntry) {
				return nullptr;
			}
			RE::TESBoundObject* boundObj = invEntry->GetObject__();
			if (!boundObj) {
				return nullptr;
			}

			// if is weapon or armor, get the item's unique ID. If there's no unique id, abort.
			uint16_t uniqueID = 0;
			RE::FormType formType = boundObj->GetFormType();
			switch (formType) {
			case RE::FormType::Weapon:
			case RE::FormType::Armor:
			{
				if (!invEntry->extraLists) {
					return nullptr;
				}
				for (auto& extraList : *invEntry->extraLists) {
					if (extraList->HasType(RE::ExtraDataType::kUniqueID)) {
						auto* uniqueIDExtra = extraList->GetByType<RE::ExtraUniqueID>();
						if (uniqueIDExtra) {
							uniqueID = uniqueIDExtra->uniqueID;
							break;
						}
					}
				}
				if (uniqueID == 0) {
					return nullptr;
				}
			}
			break;
			default:
			break;
			}

			switch (formType) {
			case RE::FormType::Weapon:
			{
				std::shared_ptr<WheelItemWeapon> wheelItemweap = WheelItemMutable::CreateWheelItemMutable<WheelItemWeapon>(boundObj, uniqueID);
				return wheelItemweap;
			}
			break;
			case RE::FormType::Armor:
			{
				std::shared_ptr<WheelItemArmor> wheelItemArmo = WheelItemMutable::CreateWheelItemMutable<WheelItemArmor>(boundObj, uniqueID);
				return wheelItemArmo;
			}
			break;
			case RE::FormType::Light:
			{
				RE::TESObjectLIGH* light = boundObj->As<RE::TESObjectLIGH>();
				if (!light || !light->CanBeCarried()) {
					return nullptr;
				}
				std::shared_ptr<WheelItemLight> wheelItemLight = std::make_shared<WheelItemLight>(light);
				return wheelItemLight;
			}
			break;
			case RE::FormType::Ammo:
			{
				RE::TESAmmo* ammo = boundObj->As<RE::TESAmmo>();
				if (!ammo) {
					return nullptr;
				}
				std::shared_ptr<WheelItemAmmo> wheelItemAmmo = std::make_shared<WheelItemAmmo>(ammo);
				return wheelItemAmmo;
			}
			break;
			case RE::FormType::AlchemyItem:  // can be food, drink, poison, potion
			{
				RE::AlchemyItem* alchemyItem = boundObj->As<RE::AlchemyItem>();
				if (!alchemyItem) {
					return nullptr;
				}
				std::shared_ptr<WheelItemAlchemy> wheelItemAlchemy = std::make_shared<WheelItemAlchemy>(alchemyItem);
				return wheelItemAlchemy;
			}
			break;
			}
		} else if (ui->IsMenuOpen(RE::MagicMenu::MENU_NAME)) {
			auto* magMenu = static_cast<RE::MagicMenu*>(ui->GetMenu(RE::MagicMenu::MENU_NAME).get());
			if (!magMenu) {
				return nullptr;
			}
			RE::TESForm* form = Utils::Inventory::GetSelectedFormInMagicMenu(magMenu);
			if (!form) {
				return nullptr;
			}
			switch (form->GetFormType()) {
			case RE::FormType::Spell:
			{
				std::shared_ptr<WheelItemSpell> wheelItemSpell = std::make_shared<WheelItemSpell>(form->As<RE::SpellItem>());
				return wheelItemSpell;
			}
			break;
			case RE::FormType::Shout:
			{
				std::shared_ptr<WheelItemShout> wheelItemShout = std::make_shared<WheelItemShout>(form->As<RE::TESShout>());
				return wheelItemShout;
			}
			break;
			}
		}
	}
	catch (std::exception exception) {
		INFO("Exception: {}", exception.what());
	}

	return nullptr;
}

std::shared_ptr<WheelItem> WheelItemFactory::MakeWheelItemFromJsonObject(nlohmann::json a_json, SKSE::SerializationInterface* a_intfc)
{
	if (!a_json.contains("type")) {
		throw std::exception("WheelItemFactory::MakeWheelItemFromJsonObject: Json object does not contain a \"type\" field.");
	}
	if (!a_json.contains("formID")) {
		throw std::exception("WheelItemFactory::MakeWheelItemFromJsonObject: Json object does not contain a \"formID\" field.");
	}
	std::string type = a_json["type"];
	RE::FormID formID = a_json["formID"].get<RE::FormID>();
	if (!a_intfc->ResolveFormID(formID, formID)) {
		return nullptr;
	}
	try
	{
		if (type == WheelItemWeapon::ITEM_TYPE_STR) {
			RE::TESObjectWEAP* weap = static_cast<RE::TESObjectWEAP*>(RE::TESForm::LookupByID(formID));
			if (!weap) {
				return nullptr;
			}
			uint16_t uniqueID = a_json["uniqueID"].get<uint16_t>();
			std::shared_ptr<WheelItemWeapon> wheelItemweap = WheelItemMutable::CreateWheelItemMutable<WheelItemWeapon>(weap, uniqueID);
			return wheelItemweap;
		} else if (type == WheelItemArmor::ITEM_TYPE_STR) {
			RE::TESObjectARMO* armor = static_cast<RE::TESObjectARMO*>(RE::TESForm::LookupByID(formID));
			if (!armor) {
				return nullptr;
			}
			uint16_t uniqueID = a_json["uniqueID"].get<uint16_t>();
			std::shared_ptr<WheelItemArmor> wheelItemArmo = WheelItemMutable::CreateWheelItemMutable<WheelItemArmor>(armor, uniqueID);
			return wheelItemArmo;
		} else if (type == WheelItemSpell::ITEM_TYPE_STR) {
			RE::SpellItem* spell = static_cast<RE::SpellItem*>(RE::TESForm::LookupByID(formID));
			if (!spell) {
				return nullptr;
			}
			std::shared_ptr<WheelItemSpell> wheelItemSpell = std::make_shared<WheelItemSpell>(spell);
			return wheelItemSpell;
		} else if (type == WheelItemShout::ITEM_TYPE_STR) {
			RE::TESShout* shout = static_cast<RE::TESShout*>(RE::TESForm::LookupByID(formID));
			if (!shout) {
				return nullptr;
			}
			std::shared_ptr<WheelItemShout> wheelItemShout = std::make_shared<WheelItemShout>(shout);
			return wheelItemShout;
		} else if (type == WheelItemLight::ITEM_TYPE_STR) {
			RE::TESObjectLIGH* light = static_cast<RE::TESObjectLIGH*>(RE::TESForm::LookupByID(formID));
			if (!light) {
				return nullptr;
			}
			std::shared_ptr<WheelItemLight> wheelItemLight = std::make_shared<WheelItemLight>(light);
			return wheelItemLight;
		} else if (type == WheelItemAmmo::ITEM_TYPE_STR) {
			RE::TESAmmo* ammo = static_cast<RE::TESAmmo*>(RE::TESForm::LookupByID(formID));
			if (!ammo) {
				return nullptr;
			}
			std::shared_ptr<WheelItemAmmo> wheelItemAmmo = std::make_shared<WheelItemAmmo>(ammo);
			return wheelItemAmmo;
		} else if (type == WheelItemAlchemy::ITEM_TYPE_STR) {
			RE::AlchemyItem* alchemyItem = static_cast<RE::AlchemyItem*>(RE::TESForm::LookupByID(formID));
			if (!alchemyItem) {
				return nullptr;
			}
			std::shared_ptr<WheelItemAlchemy> wheelItemAlchemy = std::make_shared<WheelItemAlchemy>(alchemyItem);
			return wheelItemAlchemy;
		}
	}
	catch (std::exception exception) {
		INFO("Exception when de-serializing wheel item: {}", exception.what());
	}
	
	return nullptr;
}
