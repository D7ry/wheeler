#pragma once
#include "WheelItems/WheelItem.h"
constexpr std::uint32_t WHEELER_SERIALIZATION_ID = 'WHLR';
constexpr std::uint32_t WHEELER_SERIALIZATION_TYPE = 'WHLR';
class Serializer_V0
{

public:
	static void BindSerializationCallbacks(const SKSE::SerializationInterface* a_in);

protected:
	static void Save(SKSE::SerializationInterface* a_intfc);
	static void Load(SKSE::SerializationInterface* a_intfc);
	static void Revert(SKSE::SerializationInterface* a_intfc);
	static inline uint32_t _version = 0;
};
