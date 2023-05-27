#include "nlohmann/json.hpp"

#include "WheelItemFactory.h"
#include "bin/Utils.h"
#include "WheelItem.h"
#include "WheelItemSpell.h"
#include "WheelItemWeapon.h"

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

		uint16_t uniqueID = 0;
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
		
		RE::FormType formType = boundObj->GetFormType();
		if (formType == RE::FormType::Weapon) {
			std::shared_ptr<WheelItemWeapon> wheelItemweap = WheelItemMutable::CreateWheelItemMutable<WheelItemWeapon>(boundObj, uniqueID);
			//std::shared_ptr<WheelItemWeapon> wheelItemweap = std::make_shared<WheelItemWeapon>(boundObj->As<RE::TESObjectWEAP>(), uniqueID);
			return wheelItemweap;
		} else if (formType == RE::FormType::Armor) {
		
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
			std::shared_ptr<WheelItemSpell> wheelItemSpell = std::make_shared<WheelItemSpell>(form->As<RE::SpellItem>());
			return wheelItemSpell;
		}
	}
	return nullptr;
}

std::shared_ptr<WheelItem> WheelItemFactory::MakeWheelItemFromJsonObject(nlohmann::json a_json, SKSE::SerializationInterface* a_intfc)
{
	if (!a_json.contains("type")) {
		ERROR("Error: WheelItemFactory::MakeWheelItemFromJsonObject: json object does not contain \"type\" field.");
		return nullptr;
	}
	std::string type = a_json["type"];
	if (type == WheelItemWeapon::ITEM_TYPE_STR) {
		RE::FormID formID = a_json["formID"].get<RE::FormID>();
		if (!a_intfc->ResolveFormID(formID, formID)) {
			ERROR("Error: WheelItemFactory::MakeWheelItemFromJsonObject: failed to resolve formID.");
			return nullptr;
		}
		RE::TESObjectWEAP* weap = static_cast<RE::TESObjectWEAP*>(RE::TESForm::LookupByID(formID));
		if (!weap) {
			ERROR("Error: WheelItemFactory::MakeWheelItemFromJsonObject: failed to lookup weap.");
			return nullptr;
		}
		uint16_t uniqueID = a_json["uniqueID"].get<uint16_t>();
		std::shared_ptr<WheelItemWeapon> wheelItemweap = WheelItemMutable::CreateWheelItemMutable<WheelItemWeapon>(weap, uniqueID);
		return wheelItemweap;
	} else if (type == WheelItemSpell::ITEM_TYPE_STR) {
		RE::FormID formID = a_json["formID"].get<RE::FormID>();
		if (!a_intfc->ResolveFormID(formID, formID)) {
			ERROR("Error: WheelItemFactory::MakeWheelItemFromJsonObject: failed to resolve formID.");
			return nullptr;
		}
		RE::SpellItem* spell = static_cast<RE::SpellItem*>(RE::TESForm::LookupByID(formID));
		if (!spell) {
			ERROR("Error: WheelItemFactory::MakeWheelItemFromJsonObject: failed to lookup spell.");
			return nullptr;
		}
		std::shared_ptr<WheelItemSpell> wheelItemSpell = std::make_shared<WheelItemSpell>(spell);
		return wheelItemSpell;
	}
	return nullptr;
}
