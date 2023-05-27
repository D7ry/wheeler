#include "Wheel.h"
#include "WheelEntry.h"

Wheel::Wheel() {}
Wheel::~Wheel() 
{
    this->Clear();
}
void Wheel::PushEntry(std::unique_ptr<WheelEntry> a_entry) 
{
    this->entries.push_back(std::move(a_entry));
}

bool Wheel::Empty()
{
    return this->entries.empty();
}

void Wheel::Clear()
{
    this->entries.clear();
}

void Wheel::PrevItemInEntry(int a_entryIdx)
{
    if (a_entryIdx < 0 || a_entryIdx >= this->entries.size()) {
        return;
    }
    this->entries[a_entryIdx]->PrevItem();
}

void Wheel::NextItemInEntry(int a_entryIdx)
{
    if (a_entryIdx < 0 || a_entryIdx >= this->entries.size()) {
        return;
    }
    this->entries[a_entryIdx]->NextItem();

}

void Wheel::ActivateEntryPrimary(int a_entryIdx)
{
    if (a_entryIdx < 0 || a_entryIdx >= this->entries.size()) {
        return;
    }
    this->entries[a_entryIdx]->ActivateItemPrimary();
}

void Wheel::ActivateEntrySecondary(int a_entryIdx)
{
    if (a_entryIdx < 0 || a_entryIdx >= this->entries.size()) {
        return;
    }
    this->entries[a_entryIdx]->ActivateItemSecondary();
}
