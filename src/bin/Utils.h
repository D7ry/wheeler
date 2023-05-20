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

		void GetEntryExtraDataLists(std::vector<RE::ExtraDataList*>& r_ret, const std::unique_ptr<RE::InventoryEntryData>& a_invEntry);
		
		enum class Hand
		{
			Left,
			Right,
			Both,
			None
		};
		Hand GetWeaponEquippedHand(RE::Actor* a_actor, RE::TESObjectWEAP* a_weapon, uint32_t a_uniqueID);

		RE::InventoryEntryData* GetSelectedItemIninventory(RE::InventoryMenu* a_invMenu);
		RE::TESForm* GetSelectedFormInMagicMenu(RE::MagicMenu* a_magMen);

	}
	
	namespace Workaround
	{
		inline void* NiMemAlloc_1400F6B40(int size)
		{
			using func_t = void* (*)(int);
			REL::Relocation<func_t> func{ RELOCATION_ID(10798, 10854) };
			return func(size);
		}
	}

}
