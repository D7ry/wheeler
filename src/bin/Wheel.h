#pragma once
class WheelEntry;
class Wheel
{
public:
    Wheel();
    ~Wheel();
    void PushEntry(std::unique_ptr<WheelEntry> a_entry);

    void SerializeIntoJsonObj(nlohmann::json& a_json);
    static std::unique_ptr<Wheel> SerializeFromJsonObj(nlohmann::json& a_json);
    void Clear();

    void PrevItemInActiveEntry(int a_entryIdx);
    void NextItemInActiveEntry(int a_entryIdx);
private:
    std::vector<std::unique_ptr<WheelEntry>> entries = {};
};
