#include "WheelItemFactory.h"
#include "bin/Utils.h"
#include "WheelItem.h"
#include "WheelItemSpell.h"
#include "WheelItemWeapon.h"
#include "WheelItemMutableManager.h"

std::shared_ptr<WheelItem> WheelItemFactory::MakeWheelItemFromSelected()
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
			std::shared_ptr<WheelItemWeapon> wheelItemweap = std::make_shared<WheelItemWeapon>(boundObj->As<RE::TESObjectWEAP>(), uniqueID);
			WheelItemMutableManager::GetSingleton()->Track(wheelItemweap);  // track the new wheelItem
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
			std::shared_ptr<WheelItemSpeel> wheelItemSpell = std::make_shared<WheelItemSpeel>(form->As<RE::SpellItem>());
			return wheelItemSpell;
		}
	}
	return nullptr;
}
