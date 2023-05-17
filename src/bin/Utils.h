#pragma once
#include <unordered_map>
#include <PCH.h>
namespace Utils
{
	namespace Slot
	{
		RE::BGSEquipSlot* GetLeftHandSlot();
		RE::BGSEquipSlot* GetVoiceSlot();
		RE::BGSEquipSlot* GetRightHandSlot();
	}

	namespace Time
	{
		float GGTM();
		void SGTM(float a_in);
	}

}
