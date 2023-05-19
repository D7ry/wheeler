#include "WheelItemFactory.h"
#include "bin/Utils.h"
#include "WheelItem.h"
#include "WheelItemSpell.h"
#include "WheelItemWeapon.h"
#include "WheelItemMutableManager.h"

WheelItem* WheelItemFactory::MakeWheelItemFromSelected()
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
		// What the following code does:
		// 1. search for the inventory entry's unique ID
		// 2. if the entry has uniqueID, use id as an identifier for the wheel item when looking for the entry to equip
		// 3. if the entry has no uniqueID, assign it a new one.
		uint16_t uniqueID = 0;
		if (invEntry->extraLists) {
			for (auto* extraDataList : *invEntry->extraLists) {
				if (extraDataList->HasType(RE::ExtraDataType::kUniqueID)) {
					RE::ExtraUniqueID* Xid = extraDataList->GetByType<RE::ExtraUniqueID>();
					if (Xid) {
						uniqueID = Xid->uniqueID;
						break;
					}
				}
			}
		}
		if (uniqueID == 0) {  // unique ID not found, make a new uniqueID for the object.
			uniqueID = pc->GetInventoryChanges()->GetNextUniqueID();
			RE::ExtraUniqueID* Xid = new RE::ExtraUniqueID(0x14, uniqueID);  // make a new uniqueID for the object
			if (invEntry->extraLists == nullptr) {
				invEntry->extraLists = new RE::BSSimpleList<RE::ExtraDataList*>;
			}
			if (invEntry->extraLists->begin() == invEntry->extraLists->end()) {
				RE::ExtraDataList* XList = RE::ExtraDataList::ConstructExtraDataList();
			}
			invEntry->extraLists->front()->Add(Xid);
		}
		RE::FormType formType = boundObj->GetFormType();
		if (formType == RE::FormType::Weapon) {
			WheelItemWeapon* wheelItemWeap = new WheelItemWeapon(boundObj->As<RE::TESObjectWEAP>(), uniqueID);
			WheelItemMutableManager::GetSingleton()->Track(wheelItemWeap);  // track the new wheelItem
			return wheelItemWeap;
		} else if (formType == RE::FormType::Armor) {
		
		}
	}
	return nullptr;
}
