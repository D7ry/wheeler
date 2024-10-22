#include "WheelItemMutable.h"
#include "WheelItemMutableManager.h"

WheelItemMutable::~WheelItemMutable()
{
	WheelItemMutableManager::GetSingleton()->UnTrack(this);
}
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


std::pair<int, RE::ExtraDataList*> WheelItemMutable::GetItemExtraDataAndCount(RE::TESObjectREFR::InventoryItemMap& a_inv)
{
	try {
		std::pair<int, RE::ExtraDataList*> ret = { 0, nullptr };

		// 遍历Inventory寻找匹配的物品
		std::unique_ptr<RE::InventoryEntryData>* pp = nullptr;
		for (auto& [boundObj, data] : a_inv) {
			if (boundObj->formID == this->_obj->GetFormID()) {
				pp = &data.second;
			}
		}
		/*这里处理存档数据里某个物品尤其是铁制匕首铁制剑或铁制战斧莫名其妙堆叠显示数量2，扔地上但是实际上只有一把*/
		/*很奇怪的崩溃*/
		// 如果没有找到pp，直接返回
		if (!pp || !*pp || !(*pp)->extraLists) {
			return ret;  // 没有ExtraDataList，返回空结果
		}

		int cleanItemCount = 0;  //统计没有附魔、没有中毒、没有强化的物品
		uint16_t uniqueID = this->GetUniqueID();
		bool targetClean = false;

		//遍历 extraLists，确保不为空
		for (auto* extraList : *(*pp)->extraLists) {
			if (!extraList) {
				continue;  // 如果extraList为空，跳过
			}

			bool thisClean = false;

			//检查物品是否是干净的（没有附魔，没有毒，没有强化）
			if (extraList->HasType(RE::ExtraDataType::kEnchantment) && !extraList->HasType(RE::ExtraDataType::kHealth)) {
				thisClean = true;  // item is clean
			}

			if (thisClean) {
				cleanItemCount++;
			}

			//检查UniqueID是否匹配
			if (extraList->HasType(RE::ExtraDataType::kUniqueID)) {
				if (uniqueID == extraList->GetByType<RE::ExtraUniqueID>()->uniqueID) {
					ret.second = extraList;
					if (thisClean) {
						targetClean = true;
					} else {
						ret.first = 1;
						return ret;  //找到匹配项，返回结果
					}
				}
			}
		}

		if (targetClean) {
			ret.first = cleanItemCount;
		}

		return ret;  //返回最终结果
	} catch (const std::exception& e) {
		logger::error("Exception caught in WheelItemMutable::GetItemExtraDataAndCount: {}", e.what());
		return { 0, nullptr };
	}
}


void WheelItemMutable::GetItemEnchantment(RE::TESObjectREFR::InventoryItemMap& a_invMap, std::vector<RE::EnchantmentItem*>& r_enchantments)
{
	switch (this->_obj->GetFormType()) {
	case RE::FormType::Weapon:
		{
			auto weapon = static_cast<RE::TESObjectWEAP*>(this->_obj);
			if (weapon->formEnchanting) {
				r_enchantments.push_back(weapon->formEnchanting);
			}
			break;
		}
	case RE::FormType::Armor:
		{
			auto armor = static_cast<RE::TESObjectARMO*>(this->_obj);
			if (armor->formEnchanting) {
				r_enchantments.push_back(armor->formEnchanting);
			}
			break;
		}
	}
	std::unique_ptr<RE::InventoryEntryData>* pp = nullptr;
	for (auto& [boundObj, data] : a_invMap) {
		if (boundObj->formID == this->_obj->GetFormID()) {
			pp = &data.second;
			break;
		}
	}
	if (pp) {
		for (auto* extraList : *pp->get()->extraLists) {
			if (extraList->HasType(RE::ExtraDataType::kUniqueID)) {
				if (this->GetUniqueID() == extraList->GetByType<RE::ExtraUniqueID>()->uniqueID) {
					if (extraList->HasType(RE::ExtraDataType::kEnchantment)) {
						r_enchantments.push_back(extraList->GetByType<RE::ExtraEnchantment>()->enchantment);
					}
					break;
				}
			}
		}
	}
}

void WheelItemMutable::GetItemEnchantment(RE::InventoryEntryData* a_iData, std::vector<RE::EnchantmentItem*>& r_enchantments)
{
	switch (this->_obj->GetFormType()) {
	case RE::FormType::Weapon:
		{
			auto weapon = static_cast<RE::TESObjectWEAP*>(this->_obj);
			if (weapon->formEnchanting) {
				r_enchantments.push_back(weapon->formEnchanting);
			}
			break;
		}
	case RE::FormType::Armor:
		{
			auto armor = static_cast<RE::TESObjectARMO*>(this->_obj);
			if (armor->formEnchanting) {
				r_enchantments.push_back(armor->formEnchanting);
			}
			break;
		}
	}

	if (a_iData) {
		for (RE::ExtraDataList* extraList : *a_iData->extraLists) {
			if (extraList->HasType(RE::ExtraDataType::kUniqueID)) {
				if (this->GetUniqueID() == extraList->GetByType<RE::ExtraUniqueID>()->uniqueID) {
					if (extraList->HasType(RE::ExtraDataType::kEnchantment)) {
						r_enchantments.push_back(extraList->GetByType<RE::ExtraEnchantment>()->enchantment);
					}
					break;
				}
			}
		}
	}
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
