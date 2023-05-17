#pragma once
#include "WheelItem.h"
#include <vector>

class WheelEntry
{
	// this don't inherit WheelItem because it is a container of that
public:
	void DrawSlot(ImVec2 a_center, bool a_hovered);
	
	void DrawHighlight(ImVec2 a_center);

	bool IsActive();

	void PrevItem();
	void NextItem();

	WheelEntry() 
	{
		_selectedItem = 0;
	}

	~WheelEntry()
	{
		for (auto& item : _items) {
			delete item;
		}
	}
	std::vector<WheelItem*> _items;

private:
	int _selectedItem;
};
