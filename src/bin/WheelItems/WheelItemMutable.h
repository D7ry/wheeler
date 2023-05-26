#pragma once
#include "WheelItem.h"
/**
 * WheelItemMutable is a special parent class for WheelItemWeapon and WheelItemArmor.
 * Weapons and armors in player's inventory cannot be stored through their formID, as 
 * once the player "mutates" them through tempering/enchanting/poisoning, each instance
 * of them become unique.
 * 
 * For example, and iron sword and a tempered iron sword cannot be treated as the same item,
 * and when they both reside in the player's inventory, having only the formID is not
 * enough to distinguish them; the wheel won't know which one to equip/unequip.
 * 
 * Therefore, all subclasses of WheelItemMutable are tracked through an uniqueID, a 16-bit unsigned int,
 * tagged to their extraDataList. The uniqueIDs are also updated by WheelItemMutableManager if the game or 
 * other mods decide to change their uniqueIDs. 
 * 
 * Not all instances of weapons or armors come with an uniqueID. Therefore it's our responsibity to
 * assign them one, through the following:
 * 
 * 1. Hooking into the PickUpItem and AddToInventory event, and add uniqueIDs to the extraDataList of the added item, if it's a weapon or armor.
 * 2. Doing a full scan of the player's inventory every time the player loads and adding uniqueIDs to entries that are missing, in case the player uses the mod for the first time.
 * 
 */
class WheelItemMutable : public WheelItem
{
public:
	uint16_t GetUniqueID();
	void SetUniqueID(uint16_t a_id);
	RE::FormID GetFormID();
	bool IsMutable() override;

	template <class T>
	static WheelItemMutable* Create<T>(RE::TESBoundObject* a_obj, uint16_t a_uniqueID) {
		WheelItemMutable* ret = new T(a_obj, a_uniqueID);
		WheelItemMutableManager::GetSingleton()->Track(ret->shared_from_this());
		return ret;
	}


protected:

	WheelItemMutable();
	~WheelItemMutable();
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
