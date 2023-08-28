#include "PapyrusInterface.h"
#include "../Wheeler/Wheeler.h"
namespace PapyrusInterface
{
	inline auto GetWheelCount(RE::StaticFunctionTag*) -> INT32
	{
		return Wheeler::GetWheelCount();
	}

	inline auto GetWheelEntryCount(RE::StaticFunctionTag*, UINT32 wheelIndex) -> INT32
	{
		return Wheeler::GetWheelEntryCount(wheelIndex);
	}

	inline auto GetWheelEntryItemCount(RE::StaticFunctionTag*, UINT32 wheelIndex, UINT32 entryIndex) -> INT32
	{
		return Wheeler::GetWheelEntryItemCount(wheelIndex, entryIndex);
	}

	inline auto GetWheelEntryItems(RE::StaticFunctionTag*, UINT32 wheelIndex, UINT32 entryIndex) -> std::vector<RE::TESForm*>
	{
		return Wheeler::GetWheelEntryItems(wheelIndex, entryIndex);
	}

	bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm) {

		if (!vm) {
			return false;
		}

		vm->RegisterFunction("GetWheelCount", "WheelerHelper", GetWheelCount, false);
		vm->RegisterFunction("GetWheelEntryCount", "WheelerHelper", GetWheelEntryCount, false);
		vm->RegisterFunction("GetWheelEntryItemCount", "WheelerHelper", GetWheelEntryItemCount, false);
		vm->RegisterFunction("GetWheelEntryItems", "WheelerHelper", GetWheelEntryItems, false);

		return true;
	}
}
