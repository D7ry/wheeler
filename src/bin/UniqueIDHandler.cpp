#include "UniqueIDHandler.h"
#include "Utils.h"
void UniqueIDHandler::AssignUniqueIDsToAllObjects()
{
	INFO("Indexing uniqueIDs...");
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return;
	}
	auto inv = pc->GetInventory();
	for (auto& [boundObj, data] : inv) {
		auto rawCount = data.first;
		auto& entryData = data.second;
		auto ft = entryData.get()->GetObject__()->GetFormType();
		if (ft != RE::FormType::Armor && ft != RE::FormType::Weapon) {
			continue;
		}
		if (entryData->extraLists) {
			for (auto& xList : *entryData->extraLists) {
				if (xList) {
					auto count = xList->GetCount();
					rawCount -= count;
					EnsureXListUniqueness(xList);
					//auto xID = xList->GetByType<RE::ExtraUniqueID>();
					//if (!xID) {  // append XID to extraList if not present
					//	uint16_t nextID = pc->GetInventoryChanges()->GetNextUniqueID();
					//	xID = new RE::ExtraUniqueID(0x14, nextID);
					//	xList->Add(xID);
					//	INFO("Adding {} to {}", nextID, boundObj->GetName());
					//}
				}
			}
		}
		RE::ExtraDataList* xList = nullptr; // extra data list to be added
		while (rawCount-- > 0) {
			// workaround: directly adding the extradatalist doesn't work. instead we remove the item with the removal target
			// set to pc, and the hook on addItem will append the extraDatalist.
			pc->RemoveItem(boundObj, 1, RE::ITEM_REMOVE_REASON::kStoreInContainer, xList, pc);
			////pc->AddObjectToContainer(boundObj, xList, 1, nullptr);
			//xList = (RE::ExtraDataList*)Utils::Workaround::NiMemAlloc_1400F6B40(24);
			//RE::ExtraDataList::InitExtraDataList(xList);
			//uint16_t nextID = pc->GetInventoryChanges()->GetNextUniqueID();
			//auto xID = new RE::ExtraUniqueID(0x14, nextID);
			//xList->Add(xID);
			//xList->SetOwner(pc);
			//entryData->AddExtraList(xList);
			//INFO("Adding {} to {}", nextID, boundObj->GetName());
			//manager->ActivateAndDispatch(item, xList, 1);
		}
		//RE::ExtraDataList* xList;
		//while (rawCount-- > 0) {
		//	xList = 0;
		//	manager->ActivateAndDispatch(item, xList, 1);
		//	entryData->AddExtraList(xList);
		//}
		// What the following code does:
		// 1. search for the inventory entry's unique ID
		// 2. if the entry has uniqueID, use id as an identifier for the wheel item when looking for the entry to equip
		// 3. if the entry has no uniqueID, assign it a new one.
		//uint16_t uniqueID = 0;
		//if (invEntry->extraLists) {
		//	for (auto* extraDataList : *invEntry->extraLists) {
		//		if (extraDataList->HasType(RE::ExtraDataType::kUniqueID)) {
		//			RE::ExtraUniqueID* Xid = extraDataList->GetByType<RE::ExtraUniqueID>();
		//			if (Xid) {
		//				uniqueID = Xid->uniqueID;
		//				INFO("Found uniqueID for {} : {}", invEntry->GetDisplayName(), uniqueID);
		//				break;
		//			}
		//		}
		//	}
		//}
		//if (uniqueID == 0) {  // unique ID not found, make a new uniqueID for the object.
		//	uniqueID = pc->GetInventoryChanges()->GetNextUniqueID();
		//	INFO("Unable to find unique ID for {}, assigning new uniqueID: {}", invEntry->GetDisplayName(), uniqueID);
		//	RE::ExtraUniqueID* Xid = new RE::ExtraUniqueID(0x14, uniqueID);  // make a new uniqueID for the object
		//	RE::ExtraDataList* xList;

		//	invEntry->AddExtraList();
			//if (invEntry->extraLists == nullptr) {                           // no extraLists to store
			//	invEntry->extraLists = new RE::BSSimpleList<RE::ExtraDataList*>;
			//}
			//
			//if (invEntry->extraLists->empty()) {  // no extraList to store
			//	RE::ExtraDataList* XList = (RE::ExtraDataList*)Utils::Workaround::NiMemAlloc_1400F6B40(24);
			//	RE::ExtraDataList::InitExtraDataList(XList);
			//	invEntry->extraLists->push_front(XList);
			//}
			//invEntry->extraLists->front()->Add(Xid);
		
	}
}

void UniqueIDHandler::EnsureXListUniqueness(RE::ExtraDataList*& a_extraList)
{
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		return;
	}
	auto invChanges = pc->GetInventoryChanges();
	if (!invChanges) {
		return;
	}
	
	if (a_extraList == nullptr) {
		a_extraList = (RE::ExtraDataList*)Utils::Workaround::NiMemAlloc_1400F6B40(24);
		RE::ExtraDataList::InitExtraDataList(a_extraList);
	}

	if (!a_extraList->HasType(RE::ExtraDataType::kUniqueID)) {
		uint16_t nextID = invChanges->GetNextUniqueID();
		auto xID = new RE::ExtraUniqueID(0x14, nextID);
		a_extraList->Add(xID);
	}

}
