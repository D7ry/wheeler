#pragma once
#include "WheelItem.h"

class WheelItemWeapon : public WheelItem
{
public:
	using WheelItem::WheelItem;
	void Draw(ImVec2 a_center, bool a_hovered) override;

private:
		
};
