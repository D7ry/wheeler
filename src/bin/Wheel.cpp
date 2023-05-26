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