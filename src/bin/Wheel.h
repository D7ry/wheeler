#pragma once
#include <shared_mutex>
#include "nlohmann/json.hpp"
#include "WheelEntry.h"

class Wheel
{
public:
    Wheel();
    ~Wheel();

	void Draw(ImVec2 a_wheelCenter, float a_cursorAngle, bool a_cursorCentered, RE::TESObjectREFR::InventoryItemMap& a_imap);
		

    void Clear();
    bool Empty();
    
    void PushEntry(std::unique_ptr<WheelEntry> a_entry);
	void PushEmptyEntry();

    void PrevItemInHoveredEntry();
    void NextItemInHoveredEntry();
	
	void ResetAnimation();

    void ActivateHoveredEntryPrimary(bool a_editMode);

	/// <summary>
	/// Activate the entry using a secondary input(mouse right click / controller left trigger), which either deletes
	/// an item in the entry, or the whole entry when it's empty.
	/// 
	/// </summary>
	/// <param name="a_editMode">Whether we're in edit mode, which prompts us to deletion.</param>
	void ActivateHoveredEntrySecondary(bool a_editMode);


	void MoveHoveredEntryForward();
	void MoveHoveredEntryBack();

    void SerializeIntoJsonObj(nlohmann::json& a_json);
	static std::unique_ptr<Wheel> SerializeFromJsonObj(const nlohmann::json& a_json, SKSE::SerializationInterface* a_intfc);

	void SetHoveredEntryIndex(int a_index);

private:
    std::vector<std::unique_ptr<WheelEntry>> _entries = {};
	std::shared_mutex _lock;
	
	// currently active item, will be highlighted. Gets reset every time wheel reopens.
	int _hoveredEntryIdx = -1;
	
};
