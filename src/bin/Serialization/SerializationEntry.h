#pragma once
constexpr std::uint32_t WHEELER_SERIALIZATION_ID = 'WHLR';
constexpr std::uint32_t WHEELER_JSON_STRING_TYPE = 'WJSN';
constexpr std::uint32_t SERIALIZER_VERSION = 2;
/// <summary>
/// Handles serialization interface callbacks. This class does not turn current datastructure into serialized data,
///  but rather serves as a caller to Wheeler::SerializeIntoJsonObj and Wheeler::SerializeFromJsonObj, which does 
/// further calls that traverse the data hierarchy to serialize and de-serialize.
/// For serialization, the result for serialization traversal is then dumped as a string by this class, and then written into the save file.
/// For de-serialization, we traverse the hierarchy the same way and build data structure.
/// </summary>
class SerializationEntry
{

public:
	// Set up serialization
	static void BindSerializationCallbacks(const SKSE::SerializationInterface* a_in);

protected:
	static void Save(SKSE::SerializationInterface* a_intfc);
	static void Load(SKSE::SerializationInterface* a_intfc);
	static void Revert(SKSE::SerializationInterface* a_intfc);
};
