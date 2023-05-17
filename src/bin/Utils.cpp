#include "Utils.h"
namespace Utils
{
	namespace Slot
	{
		RE::BGSEquipSlot* GetLeftHandSlot()
		{
			using func_t = decltype(&GetLeftHandSlot);
			const REL::Relocation<func_t> func{ RELOCATION_ID(23150, 23607) };
			return func();
		}
		RE::BGSEquipSlot* GetVoiceSlot()
		{
			using func_t = decltype(&GetVoiceSlot);
			const REL::Relocation<func_t> func{ RELOCATION_ID(23153, 23610) };
			return func();
		}

		RE::BGSEquipSlot* GetRightHandSlot()
		{
			using func_t = decltype(&GetRightHandSlot);
			const REL::Relocation<func_t> func{ RELOCATION_ID(23151, 23608) };
			return func();
		}
		void CleanSlot(RE::PlayerCharacter* a_pc, RE::BGSEquipSlot* a_slot)
		{
			RE::ActorEquipManager* aem = RE::ActorEquipManager::GetSingleton();
			if (!aem) {
				return;
			}
			auto* dummy = RE::TESForm::LookupByID<RE::TESForm>(0x00020163)->As<RE::TESObjectWEAP>();
			//sound false, queue false, force true
			aem->EquipObject(a_pc, dummy, nullptr, 1, a_slot, false, true, false);
			aem->UnequipObject(a_pc, dummy, nullptr, 1, a_slot, false, true, false);
		}
	}

	namespace Time
	{
		float GGTM()
		{
			static float* g_SGTM = (float*)RELOCATION_ID(511883, 388443).address();
			return *g_SGTM;
		}
		void SGTM(float a_in)
		{
			static float* g_SGTM = (float*)RELOCATION_ID(511883, 388443).address();
			*g_SGTM = a_in;
			using func_t = decltype(SGTM);
			REL::Relocation<func_t> func{ RELOCATION_ID(66989, 68246) };
			return;
		}
	}
}
