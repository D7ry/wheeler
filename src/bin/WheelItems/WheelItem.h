#pragma once
#include "bin/Texture.h"
#include "bin/Config.h"
class ImVec2;
class WheelItem
{
public:
	WheelItem(){};
	WheelItem(Texture::Image tex)
	{
		_texture = tex;
	}
	// draw the item on the wheel's slot
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered);
	// draw the item on the wheel's highlight region(given the item is highlighted) e.g. the center of the wheel(GTA-V ish)
	virtual void DrawHighlight(ImVec2 a_center);
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv);

	/// <summary>
	/// Whether the item is available. Unavailable items are greyed out and are typically
	/// due to previous bound item being removed from inventory.
	/// </summary>
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv);

	virtual void ActivateItemLeft();
	virtual void ActivateItemRight();

protected:
	Texture::Image _texture;
};
