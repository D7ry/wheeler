#pragma once
#include "WheelItems/WheelItem.h"
class Serializer
{
public:
	/// <summary>
	/// Populates the wheelItems vectors with serialized items of the current loaded player. Called on game start/save load.
	/// </summary>
	static void LoadSave(std::vector<WheelItem*>& r_wheelItems);

	/// <summary>
	/// Load config and flush changes to Config.h
	/// </summary>
	static void LoadConfig();
};
