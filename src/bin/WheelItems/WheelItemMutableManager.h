#pragma once
#include <unordered_set>
class WheelItemMutable;
class WheelItemMutableManager : public RE::BSTEventSink<RE::TESUniqueIDChangeEvent>
{
public:
	void Register()
	{
		auto events = RE::ScriptEventSourceHolder::GetSingleton();
		events->AddEventSink(this);
	}

	static WheelItemMutableManager* GetSingleton()
	{
		static WheelItemMutableManager singleton;
		return &singleton;
	}

	/// <summary>
	/// Track a mutable wheel item. Must be called following initialization of a WheelItemMutable
	/// </summary>
	void Track(WheelItemMutable* a_mutable);

	/// <summary>
	/// Clear the tracked items. Right before reloading the wheels.
	/// </summary>
	void Clear();
	
private:
	std::unordered_set<WheelItemMutable*> _mutables = {};
	
	using EventResult = RE::BSEventNotifyControl;
	virtual EventResult ProcessEvent(const RE::TESUniqueIDChangeEvent* a_event, RE::BSTEventSource<RE::TESUniqueIDChangeEvent>* a_dispatcher) override;
};
