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

		/// <summary>
		/// Gets all extradatalists of one inventory entry. 
		/// Inventory entries in Skyrim is designed as follows:
		/// Each entry has its base information stored directly in its form. However, once the entry is modified
		/// by player(e.g. smithing, enchanting, poisoning), the item the entry is associated with gains an ExtraData.
		/// The trick is that items of the same form(for example, 2 iron swords), albeit their different tempering/enchants,
		///  are stored in the same InventoryEntryData. Their modifications are in turn stored in 2 extradatlists of the given entry.
		/// </summary>
		void GetEntryExtraDataLists(std::vector<RE::ExtraDataList*>& r_ret, const std::unique_ptr<RE::InventoryEntryData>& a_invEntry)
		{
			if (a_invEntry->extraLists == nullptr) {
				return;
			}
			for (RE::ExtraDataList* extraDataList : *a_invEntry->extraLists) {
				if (extraDataList == nullptr) {
					continue;
				}
				r_ret.push_back(extraDataList);
			}			
		}

		uint16_t GetNextUniqueID()
		{
			auto pc = RE::PlayerCharacter::GetSingleton();
			if (!pc) {
				return 0;
			}
			auto invc = pc->GetInventoryChanges();
			if (!invc) {
				return 0;
			}
			return invc->GetNextUniqueID();
		}

		RE::ExtraHealth GetExtraHealth(RE::ExtraDataList* a_list)
		{
			return *a_list->GetByType<RE::ExtraHealth>();
		}
		RE::ExtraEnchantment GetExtraEnchant(RE::ExtraDataList* a_list)
		{
			return *a_list->GetByType<RE::ExtraEnchantment>();
		}
		RE::ExtraPoison GetExtraPoison(RE::ExtraDataList* a_list)
		{
			return *a_list->GetByType<RE::ExtraPoison>();
		}

		Hand GetWeaponEquippedHand(RE::Actor* a_actor, RE::TESObjectWEAP* a_weapon, uint32_t a_uniqueID, bool itemClean)
		{
			if (!a_actor) {
				return Hand::None;
			}
			bool lhsEquipped = false, rhsEquipped = false;
			bool lhsEquippedBase = false, rhsEquippedBase = false;
			RE::InventoryEntryData* lhs = a_actor->GetEquippedEntryData(true);
			if (lhs && lhs->GetObject__() && lhs->GetObject__()->GetFormID() == a_weapon->GetFormID()) {
				if (lhs->extraLists) {
					for (auto* extraList : *lhs->extraLists) {
						if (!extraList->HasType(RE::ExtraDataType::kEnchantment) 
							&& !extraList->HasType(RE::ExtraDataType::kHealth) 
							&& !extraList->HasType(RE::ExtraDataType::kPoison)
							) {
							lhsEquippedBase = true;
						}
						if (extraList->HasType(RE::ExtraDataType::kUniqueID) && extraList->GetByType<RE::ExtraUniqueID>()->uniqueID == a_uniqueID) {
							lhsEquipped = true;
							break;
						}
					}
				}
			}
			auto rhs = a_actor->GetEquippedEntryData(false);
			if (rhs && rhs->GetObject__() && rhs->GetObject__()->GetFormID() == a_weapon->GetFormID()) {
				if (rhs->extraLists) {
					for (auto* extraList : *rhs->extraLists) {
						if (!extraList->HasType(RE::ExtraDataType::kEnchantment) 
							&& !extraList->HasType(RE::ExtraDataType::kHealth) 
							&& !extraList->HasType(RE::ExtraDataType::kPoison)) {
							rhsEquippedBase = true;
						}
						if (extraList->HasType(RE::ExtraDataType::kUniqueID) && extraList->GetByType<RE::ExtraUniqueID>()->uniqueID == a_uniqueID) {
							rhsEquipped = true;
							break;
						}
					}
				}
			}
			if (itemClean) {
				if (lhsEquippedBase && rhsEquippedBase) {
					return Hand::Both;
				} else if (lhsEquippedBase) {
					return Hand::Left;
				} else if (rhsEquippedBase) {
					return Hand::Right;
				}
			}
			if (lhsEquipped && rhsEquipped) {
				return Hand::Both;
			} else if (lhsEquipped) {
				return Hand::Left;
			} else if (rhsEquipped) {
				return Hand::Right;
			}
			return Hand::None;
		}
		RE::InventoryEntryData* GetSelectedItemIninventory(RE::InventoryMenu* a_invMenu)
		{
			if (!a_invMenu) {
				return nullptr;
			}
			RE::ItemList* il = a_invMenu->itemList;
			if (!il) {
				return nullptr;
			}
			RE::ItemList::Item* selectedItem = il->GetSelectedItem();
			if (!selectedItem) {
				return nullptr;
			}
			RE::InventoryEntryData* invEntry = selectedItem->data.objDesc;
			return invEntry;
		}
		RE::TESForm* GetSelectedFormInMagicMenu(RE::MagicMenu* a_magMen)
		{
			if (a_magMen) {
				RE::GFxValue result;
				a_magMen->uiMovie->GetVariable(&result, "_root.Menu_mc.inventoryLists.itemList.selectedEntry.formId");
				if (result.GetType() == RE::GFxValue::ValueType::kNumber) {
					RE::FormID formID = static_cast<std::uint32_t>(result.GetNumber());
					return RE::TESForm::LookupByID(formID);
				}
			}
			return nullptr;
		}
	}
}

void Utils::Magic::GetMagicItemDescription(RE::ItemCard* a_itemCard, RE::MagicItem* a_magicItem, RE::BSString& a_str)
{
	using func_t = void* (*)(RE::ItemCard*, RE::MagicItem*, RE::BSString&);
	REL::Relocation<func_t> func{ RELOCATION_ID(51022, 51900) };
	func(a_itemCard, a_magicItem, a_str);
}

static void stripMagicItemDescriptionFormatCode(std::string& a_description)
{
	if (a_description.empty()) {
		return;
	}
	while (true) {
		size_t bracketLhs = a_description.find_first_of('<');
		if (bracketLhs == std::string::npos) {
			break; // gtfo, no more format codes
		}
		// found '<', now to find '>'
		size_t bracketRhs = a_description.find_first_of('>');
		if (bracketRhs == std::string::npos) {
			break; // shouldn't happen, an opening bracket without a closing one
		}
		size_t bracketSize = bracketRhs - bracketLhs + 1;
		a_description.erase(bracketLhs, bracketSize); // get rid of bracket.
	}

}

/// <summary>
/// Get the description of the magic item without html formatting.
/// </summary>
void Utils::Magic::GetMagicItemDescription(RE::MagicItem* a_magicItem, std::string& a_buf)
{
	RE::ItemCard card;
	RE::BSString buf;
	GetMagicItemDescription(&card, a_magicItem, buf);
	a_buf = buf.c_str();
	stripMagicItemDescriptionFormatCode(a_buf);
}

RE::NiMatrix3 Utils::Math::MatrixFromAxisAngle(float theta, const RE::NiPoint3& axis)
{
	RE::NiPoint3 a = axis;
	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);
	RE::NiMatrix3 result;

	result.entry[0][0] = cosTheta + a.x * a.x * (1 - cosTheta);
	result.entry[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
	result.entry[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;

	result.entry[1][0] = a.y * a.x * (1 - cosTheta) + a.z * sinTheta;
	result.entry[1][1] = cosTheta + a.y * a.y * (1 - cosTheta);
	result.entry[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;

	result.entry[2][0] = a.z * a.x * (1 - cosTheta) - a.y * sinTheta;
	result.entry[2][1] = a.z * a.y * (1 - cosTheta) + a.x * sinTheta;
	result.entry[2][2] = cosTheta + a.z * a.z * (1 - cosTheta);

	return result;
}
