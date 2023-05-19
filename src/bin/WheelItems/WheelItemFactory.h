#include "WheelItem.h"
class WheelItemFactory
{
public:
	/// <summary>
	/// Creates a new wheel item that's either a:
	/// WheelItemSpell, WheelItemWeapon, WheelItemArmor, WheelItemAmmo, WheelItemPower, WheelItemShout,
	/// based on the item that's currently being hovered on in the inventory.
	/// Returns nullptr if no item is being currently hovered, or the item do not match any of the above types.
	/// </summary>
	/// <returns></returns>
	static WheelItem* MakeWheelItemFromSelected();
};
