#include "WheelItemMutableManager.h"
#include "WheelItemMutable.h"
using EventResult = RE::BSEventNotifyControl;

void WheelItemMutableManager::Track(std::weak_ptr<WheelItemMutable> a_mutable)
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);
	this->_mutables.push_back(a_mutable);
}

void WheelItemMutableManager::UnTrack(std::weak_ptr<WheelItemMutable> a_mutable)
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);
	if (a_mutable.expired()) {
		ERROR("WheelItemMutableManager::UnTrack: a_mutable is null");
		return;
	}
	for (auto it = this->_mutables.begin(); it != this->_mutables.end(); ++it) {
		if (it->expired()) {
			continue;
		}
		if (it->lock() == a_mutable.lock()) {
			this->_mutables.erase(it);
			return;
		}
	}
}

void WheelItemMutableManager::Clear()
{
	std::unique_lock<std::shared_mutex> lock(this->_lock);
	this->_mutables.clear();
}

EventResult WheelItemMutableManager::ProcessEvent(const RE::TESUniqueIDChangeEvent* a_event, RE::BSTEventSource<RE::TESUniqueIDChangeEvent>* a_dispatcher)
{
	std::shared_lock<std::shared_mutex> lock(this->_lock);
	if (!a_event) {
		return EventResult::kContinue;
	}
	const auto playerRefID = 0x14;
	if ((a_event->oldBaseID != playerRefID && a_event->newBaseID != playerRefID) || a_event->oldUniqueID == a_event->newUniqueID) {
		return EventResult::kContinue;
	}
	auto form = RE::TESForm::LookupByID(a_event->objectID);
	if (!form) {
		return EventResult::kContinue;
	}
	uint16_t oldUniqueID = a_event->oldUniqueID;
	uint16_t newUniqueID = a_event->newUniqueID;
	for (auto& item : this->_mutables) {
		if (item.expired()) {
			continue; // this should never happen because object removes itself from manager when it's destroyed
		}
		if (item.lock()->GetFormID() == form->GetFormID()) {
			if (item.lock()->GetUniqueID() == oldUniqueID) {
				INFO("{}'s new unique id changed from {} to {} due to external changes.", form->GetName(), oldUniqueID, newUniqueID);
				item.lock()->SetUniqueID(newUniqueID);  // update the uniqueID of the item
			}
		}
	}
	return EventResult::kContinue;
}
