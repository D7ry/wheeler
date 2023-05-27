#include "Wheel.h"
#include "WheelEntry.h"

Wheel::Wheel() {}
Wheel::~Wheel() 
{
    this->Clear();
}
void Wheel::PushEntry(std::unique_ptr<WheelEntry> a_entry) 
{
	std::unique_lock<std::shared_mutex> lock(_lock);
    this->_entries.push_back(std::move(a_entry));
}

void Wheel::PushEmptyEntry()
{
	this->PushEntry(std::make_unique<WheelEntry>());  // no need for lock here since it's already locked in PushEntry
}

bool Wheel::Empty()
{
	std::shared_lock<std::shared_mutex> lock(_lock);
    return this->_entries.empty();
}

void Wheel::Clear()
{
	std::unique_lock<std::shared_mutex> lock(_lock);
    this->_entries.clear();
}

void Wheel::PrevItemInEntry(int a_entryIdx)
{
    if (a_entryIdx < 0 || a_entryIdx >= this->_entries.size()) {
        return;
    }
    this->_entries[a_entryIdx]->PrevItem();
}

void Wheel::NextItemInEntry(int a_entryIdx)
{
    if (a_entryIdx < 0 || a_entryIdx >= this->_entries.size()) {
        return;
    }
    this->_entries[a_entryIdx]->NextItem();

}

void Wheel::ActivateEntryPrimary(int a_entryIdx, bool a_editMode)
{
	std::shared_lock<std::shared_mutex> lock(_lock);  // might involve editing the wheel, so unique lock
    if (a_entryIdx < 0 || a_entryIdx >= this->_entries.size()) {
        return;
    }
	this->_entries[a_entryIdx]->ActivateItemPrimary(a_editMode);
}

bool Wheel::ActivateEntrySecondary(int a_entryIdx, bool a_editMode)
{
	std::unique_lock<std::shared_mutex> lock(_lock); // might involve editing the wheel, so unique lock
    if (a_entryIdx < 0 || a_entryIdx >= this->_entries.size()) {
        return;
    }
	std::unique_ptr<WheelEntry>& entry = this->_entries[a_entryIdx];
	if (entry->IsEmpty()) { // remove the entry if it's empty
		this->_entries.erase(this->_entries.begin() + a_entryIdx);
		return;
	}
	this->_entries[a_entryIdx]->ActivateItemSecondary(a_editMode);
}

void Wheel::MoveEntryForward(int a_entryIdx)
{
	std::unique_lock<std::shared_mutex> lock(_lock);  // might involve editing the wheel, so unique lock
	if (a_entryIdx < 0 || this->Empty()) {
		return;
	}
	int target = a_entryIdx == this->_entries.size() - 1 ? 0 : a_entryIdx + 1;  // wrap around
	std::swap(this->_entries[a_entryIdx], this->_entries[target]);
}

void Wheel::MoveEntryBack(int a_entryIdx)
{
	std::unique_lock<std::shared_mutex> lock(_lock);  // might involve editing the wheel, so unique loc
	if (a_entryIdx < 0 || this->Empty()) {
		return;
	}
	int target = a_entryIdx == 0 ? this->_entries.size() - 1 : a_entryIdx - 1;  // wrap around
	std::swap(this->_entries[a_entryIdx], this->_entries[target]);
}
