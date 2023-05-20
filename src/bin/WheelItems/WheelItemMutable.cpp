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

bool WheelItemMutable::IsMutable()
{
	return true;
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
		int cleanItemCount = 0;  // items not modified by enchantment, poison or tempering
		uint16_t uniqueID = this->GetUniqueID();
		// iterate through the entry, searching for extradata
		bool targetClean = false;
		for (auto* extraList : *pp->get()->extraLists) {
			bool thisClean = false;
			if (!extraList->HasType(RE::ExtraDataType::kEnchantment)
				&& !extraList->HasType(RE::ExtraDataType::kPoison)
				&& !extraList->HasType(RE::ExtraDataType::kHealth)) {
				thisClean = true; // itme is not modified
			}
			if (thisClean) {
				cleanItemCount++;
			}
			if (extraList->HasType(RE::ExtraDataType::kUniqueID)) {
				if (uniqueID == extraList->GetByType<RE::ExtraUniqueID>()->uniqueID) {
					ret.second = extraList;
					if (thisClean) {
						targetClean = true;
					} else { // item with matching uniqueID isn't clean, item therefore can't stack.
						ret.first = 1;
						return ret;
					}
				}
			}
		}
		if (targetClean) {
			ret.first = cleanItemCount;
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
