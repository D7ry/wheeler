#include "nlohmann/json.hpp"

#include "SerializationEntry.h"
#include "bin/Wheeler/Wheeler.h"

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
	nlohmann::json j_wheeler;
	Wheeler::SerializeIntoJsonObj(j_wheeler);
	
	std::string writeBuffer = j_wheeler.dump();
	INFO("Serializing following record: {}", writeBuffer);
	
	Serial::Write(a_intfc, writeBuffer);

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
	std::string readBuffer;

	Serial::Read(a_intfc, readBuffer);
	INFO("Read str: {}", readBuffer);
	try {
		nlohmann::json j_wheeler = nlohmann::json::parse(readBuffer);
		Wheeler::Clear();
		Wheeler::SerializeFromJsonObj(j_wheeler, a_intfc);
	} catch (const std::exception& e) {
		INFO("Failed to parse json: {}", e.what());
		return;
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
