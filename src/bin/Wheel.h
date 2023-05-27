#pragma once
class WheelEntry;
class Wheel
{
public:
    Wheel();
    ~Wheel();

    void Clear();
    bool Empty();
    
    void PushEntry(std::unique_ptr<WheelEntry> a_entry);

    void PrevItemInEntry(int a_entryIdx);
    void NextItemInEntry(int a_entryIdx);

    void ActivateEntryPrimary(int a_entryIdx);
	void ActivateEntrySecondary(int a_entryIdx);

    void SerializeIntoJsonObj(nlohmann::json& a_json);
    static std::unique_ptr<Wheel> SerializeFromJsonObj(nlohmann::json& a_json);

private:
    std::vector<std::unique_ptr<WheelEntry>> entries = {};
};
