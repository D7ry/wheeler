#include "Hooks.h"
#include "UniqueIDHandler.h"
//namespace RE
//{
//	BaseExtraList::~BaseExtraList()
//	{
//		while (data) {
//			auto xData = data;
//			data = xData->next;
//			delete xData;
//		}
//		data = nullptr;
//		free(presence);
//		presence = nullptr;
//
//		BaseExtraListEx::Dtor(this);
//		stl::memzero(this);
//	}
//}
namespace Hooks
{
	namespace
	{
		////https://github.com/ahzaab/iEquipUtil/blob/master/src/BaseExtraListEX.cpp
		//class BaseExtraListEx
		//{
		//public:
		//	static void InstallHooks()
		//	{
		//		// Hook the Dtor that is now in the vtable and call that when the Dtor here is called
		//		REL::Relocation<std::uintptr_t> vTable(IEquip::Offsets::BaseExtraListEx::Vtbl);
		//		BaseExtraListEx::_Dtor = vTable.write_vfunc(0x0, &BaseExtraListEx::Dtor);
		//	}
		//	static void Dtor(RE::BaseExtraList* a)
		//	{
		//		BaseExtraListEx::_Dtor(a);
		//	}
		//	using Dtor_t = decltype(&BaseExtraListEx::Dtor);
		//	static inline REL::Relocation<Dtor_t> _Dtor;
		//};
		
		class PlayerCharacterEx : public RE::PlayerCharacter
		{
		public:
			static void InstallHooks()
			{
				REL::Relocation<std::uintptr_t> vTable(RE::PlayerCharacter::VTABLE[0]);
				//_RemoveItem = vTable.write_vfunc(0x56, &PlayerCharacterEx::Hook_RemoveItem);
				_AddObjectToContainer = vTable.write_vfunc(0x5A, &PlayerCharacterEx::Hook_AddObjectToContainer);
				_PickUpObject = vTable.write_vfunc(0xCC, &PlayerCharacterEx::Hook_PickUpObject);

				logger::info("Installed hooks for ({})"sv, typeid(PlayerCharacterEx).name());
			}

		private:
			//using RemoveItem_t = decltype(&RE::PlayerCharacter::RemoveItem);  // 56
			//inline static REL::Relocation<RemoveItem_t> _RemoveItem;

			using AddObjectToContainer_t = decltype(&RE::PlayerCharacter::AddObjectToContainer);  // 5A
			inline static REL::Relocation<AddObjectToContainer_t> _AddObjectToContainer;

			using PickUpObject_t = decltype(&RE::PlayerCharacter::PickUpObject);  // CC
			inline static REL::Relocation<PickUpObject_t> _PickUpObject;

			void Hook_AddObjectToContainer(RE::TESBoundObject* a_object, RE::ExtraDataList* a_extraList, int32_t a_count, RE::TESObjectREFR* a_fromRefr)
			{
				auto ft = a_object->GetFormType();
				if (a_count <= 0 || !a_object || (ft != RE::FormType::Weapon && ft != RE::FormType::Armor) ) {
					return _AddObjectToContainer(this, a_object, a_extraList, a_count, a_fromRefr);
				}

				auto countLeft = a_count;

				if (a_extraList) {
					auto count = a_extraList->GetCount();
					countLeft -= count;
					UniqueIDHandler::EnsureXListUniqueness(a_extraList);
					_AddObjectToContainer(this, a_object, a_extraList, count, a_fromRefr);
				}

				while (countLeft-- > 0) {
					a_extraList = 0;
					_AddObjectToContainer(this, a_object, a_extraList, 1, a_fromRefr);
				}

				auto invChanges = this->GetInventoryChanges();

				if (invChanges && invChanges->entryList) {
					for (auto& entry : *invChanges->entryList) {
						auto item = entry->object;

						if (item == a_object) {
							auto rawCount = entry->countDelta;

							if (entry->extraLists) {
								for (auto& xList : *entry->extraLists) {
									if (xList) {
										auto count = xList->GetCount();
										rawCount -= count;
										UniqueIDHandler::EnsureXListUniqueness(xList);
									}
								}
							}

							RE::ExtraDataList* xList = nullptr;
							while (rawCount-- > 0) {
								xList = 0;
								UniqueIDHandler::EnsureXListUniqueness(xList);
								entry->AddExtraList(xList);
							}
						}
					}
				}
			}

			//https://github.com/ahzaab/iEquipUtil/commit/34ff707f1a2654eded7023dd66237e4f6d5df578
			void Hook_PickUpObject(RE::TESObjectREFR* a_object, uint32_t a_count, bool a_arg3, bool a_playSound)
			{
				if (!a_object) {
					return _PickUpObject(this, a_object, a_count, a_arg3, a_playSound);
				}
				auto bo = a_object->GetBaseObject();
				if (!bo) {
					return _PickUpObject(this, a_object, a_count, a_arg3, a_playSound);
				}
				
				auto ft = bo->GetFormType();

				if (a_count <= 0 || (ft != RE::FormType::Weapon && ft != RE::FormType::Armor)) {
					return _PickUpObject(this, a_object, a_count, a_arg3, a_playSound);
				}
				_PickUpObject(this, a_object, a_count, a_arg3, a_playSound);
				auto countLeft = a_count;

				auto count = a_object->extraList.GetCount();
				countLeft -= count;
				auto ptr = &a_object->extraList;
				UniqueIDHandler::EnsureXListUniqueness(ptr);
				//manager->ActivateAndDispatch(a_object->GetBaseObject(), a_object->extraList, count);
				//_PickUpObject(this, a_object, count, a_arg3, a_playSound);

				while (countLeft-- > 0) { // why are we calling this?
					_AddObjectToContainer(this, a_object->GetBaseObject(), nullptr, 1, nullptr);
				}

				auto invChanges = this->GetInventoryChanges();

				if (invChanges && invChanges->entryList) {
					for (auto& entry : *invChanges->entryList) {
						auto item = entry->object;

						if (item == a_object->GetBaseObject()) {
							auto rawCount = entry->countDelta;

							if (entry->extraLists) {
								for (auto& xList : *entry->extraLists) {
									if (xList) {
										auto count2 = xList->GetCount();
										rawCount -= count2;
										UniqueIDHandler::EnsureXListUniqueness(xList);
									}
								}
							}

							RE::ExtraDataList* xList;
							while (rawCount-- > 0) {
								xList = 0;
								UniqueIDHandler::EnsureXListUniqueness(xList);
								entry->AddExtraList(xList);
							}
						}
					}
				}
			}
		};
	}

	void Install()
	{
		SKSE::AllocTrampoline(1 << 5);

		//CanInput::Install();
		PlayerCharacterEx::InstallHooks();
		//BaseExtraListEx::InstallHooks();
		logger::info("Installed all hooks");
	}
}
