#include "WheelItemWeapon.h"
#include "include/lib/Drawer.h"

void WheelItemWeapon::Draw(ImVec2 a_center, bool a_hovered)
{
	//std::string text = a_hovered ? "Weapon Hovered" : "Weapon Not Hovered";
	Drawer::draw_text(a_center.x, a_center.y, 
		Config::Styling::Item::Text::OffsetX, Config::Styling::Item::Text::OffsetY,
		_weapon->GetName(), 255, 255, 255, 255,
		Config::Styling::Item::Text::Size);
	Drawer::draw_texture(_texture.texture, 
		ImVec2(a_center.x, a_center.y),
		Config::Styling::Item::Texture::OffsetX,
		Config::Styling::Item::Texture::OffsetY,
		ImVec2(_texture.width * Config::Styling::Item::Texture::Scale, _texture.height * Config::Styling::Item::Texture::Scale), 
		0);
	//PieMenu::PieMenuItem("one weapon");
}
