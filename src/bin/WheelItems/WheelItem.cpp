#include "WheelItem.h"
#include "include/lib/Drawer.h"
void WheelItem::Draw(ImVec2 a_center, bool a_hovered)
{
	Drawer::draw_text(a_center.x, a_center.y, 0, 0, "One Item", 255, 255, 255, 255);
	//Drawer::draw_element(_texture, ImVec2(0, 0), ImVec2(100, 100), 0);
}

void WheelItem::ReceiveInput(uint32_t a_keyID)
{
	// do nothing
}

bool WheelItem::IsActive()
{
	return false;
}
