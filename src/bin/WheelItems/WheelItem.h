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
	virtual void ReceiveInput(uint32_t a_keyID);
	virtual bool IsActive();

protected:
	Texture::Image _texture;
};
