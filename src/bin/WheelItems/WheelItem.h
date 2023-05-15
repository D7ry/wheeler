#pragma once
#include "bin/Texture.h"
class ImVec2;
class WheelItem
{
public:
	WheelItem(Texture::Image tex)
	{
		_texture = tex;
	}
	virtual void Draw(ImVec2 a_center, bool a_hovered);
	virtual void Activate(uint32_t a_keyID);

protected:
	Texture::Image _texture;
};

// based on 1920 x 1080 res, need to scale based on resolution
namespace WheelItemStyling
{
	namespace Texture
	{
		const inline float OffsetX = 0;
		const inline float OffsetY = -25;
		const inline float Scale = .1f;
	}

	namespace Text
	{
		const inline float OffsetX = 0;
		const inline float OffsetY = 10;
		const inline float Size = 20;
	}

};
