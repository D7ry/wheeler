#include "WheelItemMutable.h"

uint16_t WheelItemMutable::GetUniqueID()
{
	std::lock_guard<std::mutex> lock(_uniqueIDLock);
	return this->_uniqueID;
}

void WheelItemMutable::SetUniqueID(uint16_t a_id)
{
	std::lock_guard<std::mutex> lock(this->_uniqueIDLock);
	this->_uniqueID = a_id;
}

RE::FormID WheelItemMutable::GetFormID()
{
	return this->_obj->GetFormID();
}

std::pair<int, RE::ExtraDataList*> WheelItemMutable::GetItemData(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	std::pair<int, RE::ExtraDataList*> ret = { 0, nullptr };
	
	std::unique_ptr<RE::InventoryEntryData>* pp = nullptr;
	for (auto& [boundObj, data] : a_inv) {
		if (boundObj->formID == this->_obj->GetFormID()) {
			pp = &data.second;
		}
	}
	if (pp) {
		uint16_t uniqueID = this->GetUniqueID();
		for (auto* extraList : *pp->get()->extraLists) {
			if (extraList->HasType(RE::ExtraDataType::kUniqueID)) {
				if (uniqueID == extraList->GetByType<RE::ExtraUniqueID>()->uniqueID) {
					ret.first++;
					ret.second = extraList;
				}
			}
		}
	}
	return ret;
}

static bool filterMutableItems(RE::TESBoundObject& a_obj)
{
	switch (a_obj.GetFormType()) {
	case RE::FormType::Weapon:
	case RE::FormType::Armor:
		return true;
	}
	return false;
}
