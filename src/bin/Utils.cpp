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

	namespace Inventory
	{
		std::pair<RE::EnchantmentItem*, float> GetEntryEnchantAndHealth(const std::unique_ptr<RE::InventoryEntryData>& a_invEntry)
		{
			std::pair<RE::EnchantmentItem*, float> ret = { nullptr, -1.f };
			if (a_invEntry->extraLists == nullptr) {
				return ret;
			}
			bool foundEnchant = false, foundHealth = false;
			for (RE::ExtraDataList* extraDataList : *a_invEntry->extraLists) {
				if (extraDataList == nullptr) {
					continue;
				}
				if (extraDataList->HasType(RE::ExtraDataType::kEnchantment)) {
					auto Xench = extraDataList->GetByType<RE::ExtraEnchantment>();
					if (Xench != nullptr) {
						ret.first = Xench->enchantment;
						foundEnchant = true;
						if (foundHealth) {
							return ret;
						}
					}
				}
				if (extraDataList->HasType(RE::ExtraDataType::kHealth)) {
					auto Xhealth = extraDataList->GetByType<RE::ExtraHealth>();
					if (Xhealth != nullptr) {
						ret.second = Xhealth->health;
						foundHealth = true;
						if (foundEnchant) {
							return ret;
						}
					}
				}
			}
			return ret;
		}

		void GetEntryExtraDatas(std::vector<ItemExtraData>& r_ret, const std::unique_ptr<RE::InventoryEntryData>& a_invEntry)
		{
			if (a_invEntry->extraLists == nullptr) {
				return;
			}
			for (RE::ExtraDataList* extraDataList : *a_invEntry->extraLists) {
				// one extraDatalist should correspond to one item
				ItemExtraData data;
				data.hasEnchant = false;
				data.hasHealth = false;
				data.hasPoison = false;
				if (extraDataList != nullptr) {
					if (extraDataList->HasType(RE::ExtraDataType::kEnchantment)) {
						auto Xench = extraDataList->GetByType<RE::ExtraEnchantment>();
						if (Xench != nullptr) {
							data.hasEnchant = true;
							data.enchant = *Xench;
						}
					}
					if (extraDataList->HasType(RE::ExtraDataType::kHealth)) {
						auto Xhealth = extraDataList->GetByType<RE::ExtraHealth>();
						if (Xhealth != nullptr) {
							data.hasHealth = true;
							data.health = *Xhealth;
						}
					}
					if (extraDataList->HasType(RE::ExtraDataType::kPoison)) {  // poison won't be used for now
						auto xPoison = extraDataList->GetByType<RE::ExtraPoison>();
						if (xPoison != nullptr) {
							data.hasPoison = true;
							data.poison = *xPoison;
						}
					}
				}
				r_ret.push_back(data);
			}
		}

	}
}

