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
			throw new std::exception("invEntry->extraLists is null");
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
			throw new std::exception("uniqueID is 0, but it should have been properly assigned.");
		}
		
		RE::FormType formType = boundObj->GetFormType();
		if (formType == RE::FormType::Weapon) {
			std::shared_ptr<WheelItemWeapon> wheelItemweap = std::make_shared<WheelItemWeapon>(boundObj->As<RE::TESObjectWEAP>(), uniqueID);
			WheelItemMutableManager::GetSingleton()->Track(wheelItemweap);  // track the new wheelItem
			return wheelItemweap;
		} else if (formType == RE::FormType::Armor) {
		
		}
	}
	return nullptr;
}
