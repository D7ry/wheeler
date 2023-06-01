#include "Hooks.h"
#include "Utilities/UniqueIDHandler.h"
#include "bin/UserInput/Input.h"
#include "bin/Config.h"
#include "bin/Wheeler/Wheeler.h"
namespace Hooks
{
	namespace
	{
		////https://github.com/ahzaab/iEquipUtil/blob/master/src/BaseExtraListEX.cpp
		
		/// <summary>
		/// Hooks any action that adds item to the player's inventory. 
		/// Ensures all added items that will potentially be a WheelItemMutable(weapon & armor) have an uniqueID attached to its extraDataList.
		/// uniqueID is used to differentiate diffeent instances of a form in the player's inventory; more about it in WheelItemMutable.
		/// </summary>
		class OnChangePlayerInventory : public RE::PlayerCharacter
		{
		public:
			static void Install()
			{
				REL::Relocation<std::uintptr_t> vTable(RE::PlayerCharacter::VTABLE[0]);
				//_RemoveItem = vTable.write_vfunc(0x56, &OnChangePlayerInventory::Hook_RemoveItem);
				_AddObjectToContainer = vTable.write_vfunc(0x5A, &OnChangePlayerInventory::Hook_AddObjectToContainer);
				_PickUpObject = vTable.write_vfunc(0xCC, &OnChangePlayerInventory::Hook_PickUpObject);

				logger::info("Installed hooks for ({})"sv, typeid(OnChangePlayerInventory).name());
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
	class OnPlayerUpdate  //no longer used
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> PlayerCharacterVtbl{ RE::VTABLE_PlayerCharacter[0] };

			_Update = PlayerCharacterVtbl.write_vfunc(0xAD, Update);
			logger::info("hook:OnPlayerUpdate");
		}

	private:
		static void Update(RE::PlayerCharacter* a_this, float a_delta)
		{
			_Update(a_this, a_delta);
		}
		static inline REL::Relocation<decltype(Update)> _Update;
	};

	// https://github.com/SlavicPotato/ied-dev
	/// <summary>
	/// Hooks the input event dispatching function, this function dispatches a linked list of input events
	/// to other input event handlers, hence by modifying the linked list we can filter out unwanted input events.
	/// </summary>
	class OnInputEventDispatch
	{
	public:
		static void Install()
		{
			auto& trampoline = SKSE::GetTrampoline();
			REL::Relocation<uintptr_t> caller{ RELOCATION_ID(67315, 68617) };
			_DispatchInputEvent = trampoline.write_call<5>(caller.address() + RELOCATION_OFFSET(0x7B, 0x7B), DispatchInputEvent);
		}

	private:
		static void DispatchInputEvent(RE::BSTEventSource<RE::InputEvent*>* a_dispatcher, RE::InputEvent** a_evns)
		{
			if (!a_evns) {
				_DispatchInputEvent(a_dispatcher, a_evns);
				return;
			}

			Input::GetSingleton()->ProcessAndFilter(a_evns);
			
			_DispatchInputEvent(a_dispatcher, a_evns);
		}
		static inline REL::Relocation<decltype(DispatchInputEvent)> _DispatchInputEvent;
	};

	//https://github.com/ersh1/Precision/blob/main/src/Hooks.h
	class OnCameraUpdate
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> hook1{ RELOCATION_ID(49852, 50784) };  // 84AB90, 876700

			auto& trampoline = SKSE::GetTrampoline();
			_TESCamera_Update = trampoline.write_call<5>(hook1.address() + RELOCATION_OFFSET(0x1A6, 0x1A6), TESCamera_Update);  // 84AD36, 8768A6
		}

	private:
		static void TESCamera_Update(RE::TESCamera* a_this);

		static inline REL::Relocation<decltype(TESCamera_Update)> _TESCamera_Update;
	};

	void OnCameraUpdate::TESCamera_Update(RE::TESCamera* a_this)
	{
		_TESCamera_Update(a_this);
		// if (Wheeler::OffsetCamera(a_this)) {  // if we offset the camera, we need to update the camera
		// 	RE::NiUpdateData updateData;
		// 	a_this->cameraRoot->UpdateDownwardPass(updateData, 0);
		// }
	}

	void Install()
	{
		SKSE::AllocTrampoline(1 << 5);

		OnChangePlayerInventory::Install();
		OnInputEventDispatch::Install();
		//OnCameraUpdate::Install(); // hooking camera is a terrible idea
		logger::info("Installed all hooks");
	}
}
