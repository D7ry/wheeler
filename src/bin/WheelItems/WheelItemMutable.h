#pragma once
#include "WheelItem.h"
class WheelItemMutable : public WheelItem
{
public:
	uint16_t GetUniqueID();
	void SetUniqueID(uint16_t a_id);
	RE::FormID GetFormID();
	bool IsMutable() override;

protected:

	/// <summary>
	/// Returns the count and extradatalist of a THIS item in the inventory. 
	/// Count is 0 if this item do not exist.
	/// Count is bigger than 1 if and only if the item matching this WheelItem's uniqueID is not tempered, enchanted, or poisoned, and
	/// there exists other items(albeit with different uniqueID) that are not tempered, enchanted, or poisoned.
	/// 
	/// Note: One ambiguous behavior that I can't fix is when the user adds an pair of items(counts as one item) to the wheel
	/// (which gives them the same unique IDs), then modifies the item through enchanting/tempering. In this case the items
	/// still share the same uniqueID, but now the modified item is functinally different
	/// event to automatically assign new uniqueID to the modified item.
	/// </summary>
	/// <param name="a_inv"></param>
	/// <returns>The # of available items with functionally identical extralists in the inventory.</returns>
	std::pair<int, RE::ExtraDataList*> GetItemData(RE::TESObjectREFR::InventoryItemMap& a_inv);

	RE::TESBoundObject* _obj; // TESObjectWEAP or TESObjectARMO

private:

	uint16_t _uniqueID = 0;
	std::mutex _uniqueIDLock;
};
