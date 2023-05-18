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
		void CleanSlot(RE::PlayerCharacter* a_pc, RE::BGSEquipSlot* a_slot);
	}

	namespace Time
	{
		float GGTM();
		void SGTM(float a_in);
	}

	namespace Inventory
	{
		std::pair<RE::EnchantmentItem*, float> GetEntryEnchantAndHealth(const std::unique_ptr<RE::InventoryEntryData>& a_invEntry);
		struct ItemExtraData
		{
			bool hasEnchant;
			bool hasHealth;
			bool hasPoison;
			RE::ExtraEnchantment enchant;
			RE::ExtraHealth health;
			RE::ExtraPoison poison;
		};
		void GetEntryExtraDatas(std::vector<ItemExtraData>& r_ret, const std::unique_ptr<RE::InventoryEntryData>& a_invEntry);

		
	}

}
