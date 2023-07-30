#pragma once
#include "WheelItem.h"
class WheelItemAlchemy : public WheelItem
{
public:
	WheelItemAlchemy() = delete;
	
	WheelItemAlchemy(RE::AlchemyItem* a_alchemyItem);
	
	~WheelItemAlchemy(){};
	
	virtual void DrawSlot(ImVec2 a_center, bool a_hovered, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs) override;
	virtual void DrawHighlight(ImVec2 a_center, RE::TESObjectREFR::InventoryItemMap& a_imap, DrawArgs a_drawArgs) override;
	
	virtual bool IsActive(RE::TESObjectREFR::InventoryItemMap& a_inv) override;
	virtual bool IsAvailable(RE::TESObjectREFR::InventoryItemMap& a_inv) override;

	virtual void ActivateItemPrimary() override;
	virtual void ActivateItemSecondary() override;
	virtual void ActivateItemSpecial() override;
	
	virtual void SerializeIntoJsonObj(nlohmann::json& a_json) override;
	
	static inline const char* ITEM_TYPE_STR = "WheelItemAlchemy";

private:
	enum class WheelItemAlchemyType
	{
		kFood,
		kPotion,
		kPoison,
		kNone
	};
	RE::AlchemyItem* _alchemyItem = nullptr;
	WheelItemAlchemyType _alchemyItemType = WheelItemAlchemyType::kNone;

	void consume();
	void applyPoison();
};
