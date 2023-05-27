#pragma once
#include "WheelItems/WheelItem.h"
constexpr std::uint32_t WHEELER_SERIALIZATION_ID = 'WHLR';
constexpr std::uint32_t WHEELER_JSON_STRING_TYPE = 'WJSN';
constexpr std::uint32_t SERIALIZER_VERSION = 2;
class Serializer
{

public:
	static void BindSerializationCallbacks(const SKSE::SerializationInterface* a_in);

protected:
	static void Save(SKSE::SerializationInterface* a_intfc);
	static void Load(SKSE::SerializationInterface* a_intfc);
	static void Revert(SKSE::SerializationInterface* a_intfc);
};
