#include "Serializer.h"
#include "Wheeler.h"
#include "WheelItems/WheelEntry.h"
#include "nlohmann/json.hpp"
#include "WheelItems/WheelItem.h"

template <typename T>
bool Write(SKSE::SerializationInterface* a_interface, const T& data)
{
	return a_interface->WriteRecordData(&data, sizeof(T));
}

template <>
bool Write(SKSE::SerializationInterface* a_interface, const std::string& data)
{
	std::size_t size = data.length();
	return a_interface->WriteRecordData(size) && a_interface->WriteRecordData(data.data(), static_cast<std::uint32_t>(size));
}

template <typename T>
bool Read(SKSE::SerializationInterface* a_interface, T& result)
{
	return a_interface->ReadRecordData(&result, sizeof(T));
}

template <>
bool Read(SKSE::SerializationInterface* a_interface, std::string& result)
{
	std::size_t size = 0;
	if (!a_interface->ReadRecordData(size)) {
		return false;
	}
	if (size > 0) {
		result.resize(size);  // <--- here I replaced reverse() with resize()
		if (!a_interface->ReadRecordData(result.data(), static_cast<std::uint32_t>(size))) {
			return false;
		}
	} else {
		result = "";
	}
	return true;
}

void Serializer_V0::BindSerializationCallbacks(const SKSE::SerializationInterface* a_in)
{
	a_in->SetLoadCallback(Serializer_V0::Load);
	a_in->SetSaveCallback(Serializer_V0::Save);
	a_in->SetRevertCallback(Serializer_V0::Revert);
}
void Serializer_V0::Save(SKSE::SerializationInterface* a_intfc)
{
	INFO("Serializing wheel into save...");
	if (!a_intfc->OpenRecord(WHEELER_SERIALIZATION_ID, _version)) {
		INFO("Failed to open record");
		return;
	}
	auto wheels = Wheeler::GetWheels();
	nlohmann::json j_wheeler;
	
	nlohmann::json j_wheels;
	for (Wheeler::Wheel* wheel : wheels) {
		nlohmann::json j_wheel;
		for (WheelEntry* entry : wheel->entries) {
			nlohmann::json j_entry;
			for (std::shared_ptr<WheelItem> item : entry->GetItems()) {
				nlohmann::json j_item;
				item->SerializeInto(j_item);
				j_entry["items"].push_back(j_item);
			}
			j_entry["selecteditem"] = entry->GetSelectedItem();
			j_wheel["entries"].push_back(j_entry);
		}
		j_wheels.push_back(j_wheel);
	}
	j_wheeler["wheels"] = j_wheels;
	j_wheeler["activeentry"] = Wheeler::GetActiveEntry();
	std::string datastr = j_wheeler.dump();
	INFO("Saving following str: {}", datastr);
	

	Write(a_intfc, datastr);

	//try {
	//	json_file << j_wheeler;
	//} catch (const nlohmann::json::exception& e) {
	//	// Handle error parsing JSON
	//	ERROR("Exception dumping : {}", e.what());
	//}
	INFO("Done serializing wheel into save");
}

void Serializer_V0::Load(SKSE::SerializationInterface* a_intfc)
{
	std::string buf;
	Read(a_intfc, buf);
	INFO("Read str: {}", buf);

	//Wheeler::Clear();
	//std::vector<Wheeler::Wheel*> wheels;

	//std::string dataStr;
	//Read(a_intfc, dataStr);
	//if (dataStr.empty()) {
	//	Wheeler::SetWheels(wheels); // empty set of wheels
	//	return;
	//}
	//
	//nlohmann::json j_wheeler = nlohmann::json::parse(dataStr);
	////debugging begin
	//std::ofstream json_file("Data\\SKSE\\Plugins\\wheeler\\testLoadingDump.json");
	//try {
	//	json_file << j_wheeler;
	//} catch (const nlohmann::json::exception& e) {
	//	// Handle error parsing JSON
	//	ERROR("Exception dumping : {}", e.what());
	//	return;
	//}
	////debugging end
	//Wheeler::SetWheels(wheels);
}

void Serializer_V0::Revert(SKSE::SerializationInterface* a_intfc)
{
	Wheeler::Clear();
}

/**

"activeWheel" : 0,
"wheels" :
[
	{
		"entries":
		[
			{
				"selectedItem" ： 3
				"items:
				[
					{
						"type": "WheelItemWeapon",
						"formID": 1234(uint),
						"uniqueID": 5(uint)							
					},
					{
						"type": "WheelItemArmor",
						"formID": 1324(uint),
						"uniqueID": 6(uint)
					},
					{
						"type": "WheelItemSpell",
						"formID": 1234(uint),
					}
				]
			},
			{
				"items:
				[
				]
			},
		]
	},
	{
		"entries":
		[
			{
				"items:
				[
				]
			},
		]
	},
]
	

*/
