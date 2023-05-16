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
	virtual void Draw(ImVec2 a_center, bool a_hovered);
	virtual void ReceiveInput(uint32_t a_keyID);
	virtual bool IsActive();

protected:
	Texture::Image _texture;
};
