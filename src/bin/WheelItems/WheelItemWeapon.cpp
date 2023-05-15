#include "WheelItemWeapon.h"
#include "include/lib/Drawer.h"

void WheelItemWeapon::Draw(ImVec2 a_center, bool a_hovered)
{
	std::string text = a_hovered ? "Weapon Hovered" : "Weapon Not Hovered";
	Drawer::draw_text(a_center.x, a_center.y, 0, 0, 0, 0, text.data(), 255, 255, 255, 255);
	//PieMenu::PieMenuItem("one weapon");
}
