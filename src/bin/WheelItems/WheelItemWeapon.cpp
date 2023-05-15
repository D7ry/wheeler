#include "WheelItemWeapon.h"
#include "include/lib/Drawer.h"

void WheelItemWeapon::Draw(ImVec2 a_center, bool a_hovered)
{
	//std::string text = a_hovered ? "Weapon Hovered" : "Weapon Not Hovered";
	Drawer::draw_text(a_center.x, a_center.y, 
		WheelItemStyling::Text::OffsetX, WheelItemStyling::Text::OffsetY,
		_weapon->GetName(), 255, 255, 255, 255,
		WheelItemStyling::Text::Size);
	Drawer::draw_texture(_texture.texture, 
		ImVec2(a_center.x, a_center.y),
		WheelItemStyling::Texture::OffsetX,
		WheelItemStyling::Texture::OffsetY,
		ImVec2(_texture.width * WheelItemStyling::Texture::Scale, _texture.height * WheelItemStyling::Texture::Scale), 
		0);
	//PieMenu::PieMenuItem("one weapon");
}
