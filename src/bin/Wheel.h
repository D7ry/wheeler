#pragma once
#include "nlohmann/json.hpp"
class WheelEntry;
class Wheel
{
public:
    Wheel();
    ~Wheel();

    void Clear();
    bool Empty();
    
    void PushEntry(std::unique_ptr<WheelEntry> a_entry);
	void PushEmptyEntry();

    void PrevItemInEntry(int a_entryIdx);
    void NextItemInEntry(int a_entryIdx);

    void ActivateEntryPrimary(int a_entryIdx, bool a_editMode);

	/// <summary>
	/// Activate the entry using a secondary input(mouse right click / controller left trigger), which either deletes
	/// an item in the entry, or the whole entry when it's empty.
	/// 
	/// Returns false if an item has been deleted, true if an entry has been deleted.
	/// </summary>
	/// <param name="a_entryIdx">Index of the currently active entry.</param>
	/// <param name="a_editMode">Whether we're in edit mode, which prompts us to deletion.</param>
	/// <returns>Whether an entry has been deleted</returns>
	bool ActivateEntrySecondary(int a_entryIdx, bool a_editMode);


	void MoveEntryForward(int a_entryIdx);
	void MoveEntryBack(int a_entryIdx);

    void SerializeIntoJsonObj(nlohmann::json& a_json);
    static std::unique_ptr<Wheel> SerializeFromJsonObj(nlohmann::json& a_json);

private:
    std::vector<std::unique_ptr<WheelEntry>> _entries = {};
	std::shared_mutex _lock;
	
};
