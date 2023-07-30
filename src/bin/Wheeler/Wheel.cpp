#include "bin/Rendering/Drawer.h"

#include "Wheel.h"
#include "WheelEntry.h"
#include "bin/Config.h"

Wheel::Wheel() {}
Wheel::~Wheel() 
{
    this->Clear();
}
void Wheel::Draw(ImVec2 a_wheelCenter, float a_cursorAngle, bool a_cursorCentered, RE::TESObjectREFR::InventoryItemMap& a_imap,
	DrawArgs a_drawArgs)
{
	try {
		using namespace Config::Styling::Wheel;
		if (this->IsEmpty()) {
			Drawer::draw_text(a_wheelCenter.x, a_wheelCenter.y, "Empty Wheel", C_SKYRIMWHITE, 40.f, a_drawArgs);
			return;  // nothing more to draw
		}
		float cursorIndicatorAngle = a_cursorAngle;

		// draw entries
		using entryRuntimeData = std::pair<ImVec2, bool>;  // <entry center, is hovered>
		std::vector<entryRuntimeData> entryRuntimeDataVec;
		// draw background, cache data for foreground
		for (int entryIdx = 0; entryIdx < this->_entries.size(); entryIdx++) {
			const float entryArcSpan = 2 * IM_PI / _entries.size();

			const float innerSpacingRad = InnerSpacing / InnerCircleRadius / 2;
			float entryInnerAngleMin = entryArcSpan * (entryIdx - 0.5f) + innerSpacingRad + IM_PI / 2;
			float entryInnerAngleMax = entryArcSpan * (entryIdx + 0.5f) - innerSpacingRad + IM_PI / 2;

			float entryOuterAngleMin = entryArcSpan * (entryIdx - 0.5f) + innerSpacingRad * (InnerCircleRadius / OuterCircleRadius) + IM_PI / 2;
			float entryOuterAngleMax = entryArcSpan * (entryIdx + 0.5f) - innerSpacingRad * (InnerCircleRadius / OuterCircleRadius) + IM_PI / 2;

			if (entryInnerAngleMax > IM_PI * 2) {
				entryInnerAngleMin -= IM_PI * 2;
				entryInnerAngleMax -= IM_PI * 2;
			}

			if (entryOuterAngleMax > IM_PI * 2) {
				entryOuterAngleMin -= IM_PI * 2;
				entryOuterAngleMax -= IM_PI * 2;
			}
			bool hovered = false;
			if (!a_cursorCentered) {
				// update hovered item
				bool updatedActiveEntry = false;
				if (a_cursorAngle >= entryInnerAngleMin) {  // Normal case
					if (a_cursorAngle < entryInnerAngleMax) {
						if (entryIdx != _hoveredEntryIdx) {
							_hoveredEntryIdx = entryIdx;
							updatedActiveEntry = true;
						}
					}
				} else if (a_cursorAngle + 2 * IM_PI < entryInnerAngleMax && a_cursorAngle + 2 * IM_PI >= entryInnerAngleMin) {  // Wrap-around case
					if (entryIdx != _hoveredEntryIdx) {
						_hoveredEntryIdx = entryIdx;
						updatedActiveEntry = true;
					}
				}
				if (updatedActiveEntry) {
					RE::PlaySoundRE(Config::Sound::SD_ENTRYSWITCH);
				}
				hovered = _hoveredEntryIdx == entryIdx;
			}

			// calculate wheel center
			float t1 = (OuterCircleRadius - InnerCircleRadius) / 2;
			float t2 = InnerCircleRadius + t1;
			float rad = (entryInnerAngleMax - entryInnerAngleMin) / 2 + entryInnerAngleMin;
			ImVec2 entryCenter = ImVec2(
				a_wheelCenter.x + t2 * cosf(rad),
				a_wheelCenter.y + t2 * sinf(rad));

			if (hovered) {
				if (Config::Animation::SnappyCursorIndicator) {  // update cursor indicator angle to point to item center
					cursorIndicatorAngle = rad;
				}
			}

			int numArcSegments = (int)(256 * entryArcSpan / (2 * IM_PI)) + 1;

			std::unique_ptr<WheelEntry>& wheelEntry = _entries[entryIdx];
			// draw background first
			wheelEntry->DrawBackGround(a_wheelCenter, innerSpacingRad,
				entryInnerAngleMin, entryInnerAngleMax,
				entryOuterAngleMin, entryOuterAngleMax, hovered, numArcSegments, a_imap, a_drawArgs);
			
			// offset entry center position
			const float radiusMod = wheelEntry->GetRadiusMod();
			entryCenter.x += radiusMod * cosf(rad);
			entryCenter.y += radiusMod * sinf(rad);

			// prepare for foreground drawing
			entryRuntimeDataVec.push_back(std::make_pair(entryCenter, hovered));
		}

		// draw foreground in a separate pass to avoid overlapping
		for (int entryIdx = 0; entryIdx < this->_entries.size(); entryIdx++) {
			_entries[entryIdx]->DrawSlotAndHighlight(a_wheelCenter, entryRuntimeDataVec[entryIdx].first, entryRuntimeDataVec[entryIdx].second, a_imap, a_drawArgs);
		}

		// draw cursor indicator
		if (!a_cursorCentered) {
			float cursorIndicatorToCenterDist = InnerCircleRadius - CursorIndicatorDist;
			Drawer::draw_arc(a_wheelCenter,
				cursorIndicatorToCenterDist - (CusorIndicatorArcWidth / 2),
				cursorIndicatorToCenterDist + (CusorIndicatorArcWidth / 2),
				cursorIndicatorAngle - (CursorIndicatorArcAngle / 2), cursorIndicatorAngle + (CursorIndicatorArcAngle / 2),
				cursorIndicatorAngle - (CursorIndicatorArcAngle / 2), cursorIndicatorAngle + (CursorIndicatorArcAngle / 2),
				CursorIndicatorColor,
				32, a_drawArgs);
			ImVec2 cursorIndicatorTriPts[3] = {
				{ cursorIndicatorToCenterDist + (CusorIndicatorArcWidth / 2), +CursorIndicatorTriangleSideLength },
				{ cursorIndicatorToCenterDist + (CusorIndicatorArcWidth / 2), -CursorIndicatorTriangleSideLength },
				{ cursorIndicatorToCenterDist + (CusorIndicatorArcWidth / 2) + CursorIndicatorTriangleSideLength, 0 }
			};
			for (ImVec2& pos : cursorIndicatorTriPts) {
				pos = ImRotate(pos, cos(cursorIndicatorAngle), sin(cursorIndicatorAngle));
			}
			Drawer::draw_triangle_filled(cursorIndicatorTriPts[0] + a_wheelCenter, cursorIndicatorTriPts[1] + a_wheelCenter, cursorIndicatorTriPts[2] + a_wheelCenter, CursorIndicatorColor, a_drawArgs);
		}
	} catch (const std::exception& e) {
		logger::error("Exception in Wheel::Draw: {}", e.what());
	}
}
void Wheel::PushEntry(std::unique_ptr<WheelEntry> a_entry) 
{
	std::unique_lock<std::shared_mutex> lock(_lock);
    this->_entries.push_back(std::move(a_entry));
}

void Wheel::PushEmptyEntry()
{
	this->PushEntry(std::make_unique<WheelEntry>());  // no need for lock here since it's already locked in PushEntry
}

bool Wheel::IsEmpty()
{
	std::shared_lock<std::shared_mutex> lock(_lock);
    return this->_entries.empty();
}

void Wheel::Clear()
{
	std::unique_lock<std::shared_mutex> lock(_lock);
    this->_entries.clear();
}

void Wheel::PrevItemInHoveredEntry()
{
	if (_hoveredEntryIdx < 0 || _hoveredEntryIdx >= this->_entries.size()) {
        return;
    }
	this->_entries[_hoveredEntryIdx]->PrevItem();
}

void Wheel::NextItemInHoveredEntry()
{
	if (_hoveredEntryIdx < 0 || _hoveredEntryIdx >= this->_entries.size()) {
        return;
    }
	this->_entries[_hoveredEntryIdx]->NextItem();

}

void Wheel::ResetAnimation()
{
	std::shared_lock<std::shared_mutex> lock(_lock);
	for (auto& entry : this->_entries) {
		entry->ResetAnimation();
	}
}

void Wheel::ActivateHoveredEntryPrimary(bool a_editMode)
{
	std::shared_lock<std::shared_mutex> lock(_lock);  // might involve editing the wheel, so unique lock
	if (_hoveredEntryIdx < 0 || _hoveredEntryIdx >= this->_entries.size()) {
        return;
    }
	this->_entries[_hoveredEntryIdx]->ActivateItemPrimary(a_editMode);
}

void Wheel::ActivateHoveredEntrySecondary(bool a_editMode)
{
	std::unique_lock<std::shared_mutex> lock(_lock); // might involve editing the wheel, so unique lock
	if (_hoveredEntryIdx < 0 || _hoveredEntryIdx >= this->_entries.size()) {
        return;
    }
	std::unique_ptr<WheelEntry>& entry = this->_entries[_hoveredEntryIdx];
	if (entry->IsEmpty()) { // remove the entry if it's empty
		this->_entries.erase(this->_entries.begin() + _hoveredEntryIdx);
		return;
	}
	this->_entries[_hoveredEntryIdx]->ActivateItemSecondary(a_editMode);
}

void Wheel::ActivateHoveredEntrySpecial(bool a_editMode)
{
	std::shared_lock<std::shared_mutex> lock(_lock);
	if (_hoveredEntryIdx < 0 || _hoveredEntryIdx >= this->_entries.size() || a_editMode) {  // don't do anything if in edit mode
		return;
	}
	this->_entries[_hoveredEntryIdx]->ActivateItemSpecial(a_editMode);
}

void Wheel::MoveHoveredEntryForward()
{
	std::unique_lock<std::shared_mutex> lock(_lock);  // might involve editing the wheel, so unique lock
	if (_hoveredEntryIdx < 0 || this->_entries.empty()) {
		return;
	}
	int target = _hoveredEntryIdx == this->_entries.size() - 1 ? 0 : _hoveredEntryIdx + 1;  // wrap around
	std::swap(this->_entries[_hoveredEntryIdx], this->_entries[target]);
}

void Wheel::MoveHoveredEntryBack()
{
	std::unique_lock<std::shared_mutex> lock(_lock);  // might involve editing the wheel, so unique loc
	if (_hoveredEntryIdx < 0 || this->_entries.empty()) {
		return;
	}
	int target = _hoveredEntryIdx == 0 ? this->_entries.size() - 1 : _hoveredEntryIdx - 1;  // wrap around
	std::swap(this->_entries[_hoveredEntryIdx], this->_entries[target]);
}

void Wheel::SerializeIntoJsonObj(nlohmann::json& j_wheel)
{
	j_wheel["entries"] = nlohmann::json::array();
	for (const std::unique_ptr<WheelEntry>& entry : this->_entries) {
		nlohmann::json j_entry;
		// setup for entry
		entry->SerializeIntoJsonObj(j_entry);

		j_wheel["entries"].push_back(j_entry);
	}
}

std::unique_ptr<Wheel> Wheel::SerializeFromJsonObj(const nlohmann::json& j_wheel, SKSE::SerializationInterface* a_intfc)
{
	std::unique_ptr<Wheel> wheel = std::make_unique<Wheel>();
	nlohmann::json j_entries = j_wheel["entries"];
	for (const auto& j_entry : j_entries) {
		std::unique_ptr<WheelEntry> entry = WheelEntry::SerializeFromJsonObj(j_entry, a_intfc);
		wheel->PushEntry(std::move(entry));
	}
	return std::move(wheel);
}

void Wheel::SetHoveredEntryIndex(int a_index)
{
	this->_hoveredEntryIdx = a_index;
}

int Wheel::GetNumEntries()
{
	return this->_entries.size();
}
