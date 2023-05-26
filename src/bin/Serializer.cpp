#include "Serializer.h"
#include "Wheeler.h"
#include "WheelEntry.h"
#include "nlohmann/json.hpp"
#include "WheelItems/WheelItem.h"
#include "WheelItems/WheelItemFactory.h"

namespace Serial
{
	template <typename T>
	bool Write(SKSE::SerializationInterface* a_interface, const T& data)
	{
		return a_interface->WriteRecordData(&data, sizeof(T));
	}

	template <>
	bool Write(SKSE::SerializationInterface* a_interface, const std::string& data)
	{
		const std::size_t size = data.length();
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
			result.resize(size);
			if (!a_interface->ReadRecordData(result.data(), static_cast<std::uint32_t>(size))) {
				return false;
			}
		} else {
			result = "";
		}
		return true;
	}
}
void Serializer::BindSerializationCallbacks(const SKSE::SerializationInterface* a_in)
{
	a_in->SetLoadCallback(Serializer::Load);
	a_in->SetSaveCallback(Serializer::Save);
	a_in->SetRevertCallback(Serializer::Revert);
}
void Serializer::Save(SKSE::SerializationInterface* a_intfc)
{
	INFO("Serializing wheel into save...");
	if (!a_intfc->OpenRecord(WHEELER_JSON_STRING_TYPE, SERIALIZER_VERSION)) {
		INFO("Failed to open record");
		return;
	}
	auto wheels = Wheeler::GetWheels();
	nlohmann::json j_wheeler;
	
	nlohmann::json j_wheels = nlohmann::json::array();
	for (Wheeler::Wheel* wheel : wheels) {
		nlohmann::json j_wheel;
		j_wheel["entries"] = nlohmann::json::array();
		for (WheelEntry* entry : wheel->entries) {
			nlohmann::json j_entry;
			
			// setup for entry
			j_entry["items"] = nlohmann::json::array();
			for (std::shared_ptr<WheelItem> item : entry->GetItems()) { 
				nlohmann::json j_item;
				item->SerializeIntoJsonObj(j_item);
				j_entry["items"].push_back(j_item);
			}
			j_entry["selecteditem"] = entry->GetSelectedItem();

			j_wheel["entries"].push_back(j_entry);
		}
		j_wheels.push_back(j_wheel);
	}
	j_wheeler["wheels"] = j_wheels;
	j_wheeler["activewheel"] = Wheeler::GetActiveWheelIndex();
	std::string j_string = j_wheeler.dump();
	INFO("Serializing following record: {}", j_string);
	
	Serial::Write(a_intfc, j_string);

}

void Serializer::Load(SKSE::SerializationInterface* a_intfc)
{
	std::uint32_t type, version, length;
	if (!a_intfc->GetNextRecordInfo(type, version, length)) {
		INFO("Failed to obtain record, abort loading");
		return;
	}
	if (type != WHEELER_JSON_STRING_TYPE) {
		INFO("Load: wrong type, abort loading");
		return;
	}
	if (version != SERIALIZER_VERSION) {
		INFO("Load: wrong version, abort loading");
		return;
	}
	std::string j_string;

	Serial::Read(a_intfc, j_string);
	INFO("Read str: {}", j_string);
	
	Wheeler::Clear();

	std::vector<Wheeler::Wheel*> wheels;
	try {
		nlohmann::json j_wheeler = nlohmann::json::parse(j_string);

		nlohmann::json j_wheels = j_wheeler["wheels"];
		std::vector<Wheeler::Wheel*> wheels;
		for (const auto& j_wheel : j_wheels) {
			Wheeler::Wheel* wheel = new Wheeler::Wheel();

			nlohmann::json j_entries = j_wheel["entries"];
			for (const auto& j_entry : j_entries) {
				WheelEntry* entry = new WheelEntry();

				nlohmann::json j_items = j_entry["items"];
				for (const auto& j_item : j_items) {
					std::shared_ptr<WheelItem> item = WheelItemFactory::MakeWheelItemFromJsonObject(j_item, a_intfc);
					if (item) {
						entry->PushItem(item);
					}
				}
				entry->SetSelectedItem(j_entry["selecteditem"]);
				wheel->entries.push_back(entry);
			}
			wheels.push_back(wheel);
		}
		
		Wheeler::SetWheels(wheels);
		Wheeler::SetActiveWheelIndex(j_wheeler["activewheel"]);
	} catch (const std::exception& e) {
		INFO("Failed to deserialize wheel: {}", e.what());
		Wheeler::Clear();
		Wheeler::AddWheel(); // make a new wheel in case of serialization failure
	}
}




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


void Serializer::Revert(SKSE::SerializationInterface* a_intfc)
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
