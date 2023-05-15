#pragma once
#include "WheelItem.h"

class WheelItemWeapon : public WheelItem
{
public:
	void Draw(ImVec2 a_center, bool a_hovered) override;
};
