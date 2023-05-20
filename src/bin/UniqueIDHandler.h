#pragma once
class UniqueIDHandler
{
public:
	static void AssignUniqueIDsToAllObjects();

	/// <summary>
	/// Add a uniqueID to the given extraDatalist. If the given extradatalist is a nullptr, initialize it.
	/// Do nothing if the given extraDatalist already has a uniqueID.
	/// </summary>
	/// <param name="a_extraList"></param>
	/// <param name="a_count"></param>
	static void EnsureXListUniqueness(RE::ExtraDataList*& a_extraList);
};
