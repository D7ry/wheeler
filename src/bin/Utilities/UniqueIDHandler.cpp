#include "UniqueIDHandler.h"
#include "Utils.h"

inline static RE::ExtraDataList* InitExtraDataList(RE::ExtraDataList* a_list)
{
	using func_t = RE::ExtraDataList* (*)(RE::ExtraDataList*);
	REL::Relocation<func_t> func{ RELOCATION_ID(11437, 11583) };  
	return func(a_list);
}

void UniqueIDHandler::EnsureXListUniquenessInPcInventory()
{
	try {
		auto pc = RE::PlayerCharacter::GetSingleton();
		if (!pc) {
			return;
		}
		auto inv = pc->GetInventory();
		for (auto& [boundObj, data] : inv) {
			auto rawCount = data.first;
			auto& entryData = data.second;

			#undef GetObject
			//过滤非武器或非护甲
			auto ft = entryData.get()->GetObject()->GetFormType();
			if (ft != RE::FormType::Armor && ft != RE::FormType::Weapon) {
				continue;
			}

			//处理 ExtraDataLists
			if (entryData->extraLists) {
				for (auto& xList : *entryData->extraLists) {
					if (xList) {
						auto count = xList->GetCount();
						rawCount -= count;
						try {
							EnsureXListUniqueness(xList);
						} catch (std::exception& exception) {
							logger::error("Error occured when ensuring extraDataList uniqueness: {}, item: {}",
								exception.what(), entryData->GetObject() ? entryData->GetObject()->GetName() : "unknown");
#ifdef UNICODE
#	define GetObject GetObjectW
#else
#	define GetObject GetObjectA
#endif  // !UNICODE
						}
					}
				}
			}

			//如果还有未处理的数量，移除这些物品
			RE::ExtraDataList* xList = nullptr;// extra data list to be added
			while (rawCount-- > 0) {
				// workaround: directly adding the extradatalist doesn't work. instead we remove the item with the removal target
				// set to pc, and the hook on addItem will append the extraDatalist.
				pc->RemoveItem(boundObj, 1, RE::ITEM_REMOVE_REASON::kStoreInContainer, xList, pc);
			}
		}
	} catch (std::exception& exception) {
		logger::error("Error occured when scanning player inventory extraDataList: {}", exception.what());
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
		//RE::ExtraDataList::InitExtraDataList(a_extraList);
		InitExtraDataList(a_extraList);
	}

	if (!a_extraList->HasType(RE::ExtraDataType::kUniqueID)) {
		uint16_t nextID = invChanges->GetNextUniqueID();
		auto xID = new RE::ExtraUniqueID(0x14, nextID);
		a_extraList->Add(xID);
	}
}
