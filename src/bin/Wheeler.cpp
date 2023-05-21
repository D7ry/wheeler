#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Wheeler.h"
#include "Serializer.h"

#include "Input.h"
#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_internal.h"
#include "imgui.h"
#include "WheelItems/WheelItem.h"
#include "WheelItems/WheelItemMutable.h"
#include "WheelItems/WheelItemSpell.h"
#include "WheelItems/WheelItemWeapon.h"
#include "WheelItems/WheelEntry.h"
#include "WheelItems/WheelEntryAdder.h"
#include "WheelItems/WheelItemMutableManager.h"
#include "include/lib/Drawer.h"
#include "Controls.h"

#include "Utils.h"
namespace TestData
{
	void Load(std::vector<Wheeler::Wheel*>& _wheels)
	{
		assert(_wheels.empty());
		auto dh = RE::TESDataHandler::GetSingleton();

		//auto o1 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139A4, "Skyrim.esm"));  // glass battle axe
		//auto o2 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139B6, "Skyrim.esm"));  // daedric dagger
		//auto o3 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139B1, "Skyrim.esm"));  // ebony sword
		//auto o4 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x13987, "Skyrim.esm"));  // steel greatsword
		//auto o5 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0xA5DEF, "Skyrim.esm"));  // Gauldur Blackbow
		//auto o6 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x13998, "Skyrim.esm"));  // Dwarven mace
		//auto o7 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x10c6fb, "Skyrim.esm"));  // Dwarven mace
	
		Wheeler::Wheel* w1 = new Wheeler::Wheel();

		//// 1st entry in w1, contains 1 item
		//WheelEntry* l1 = new WheelEntry();
		//l1->_items.push_back(o1);

		//// 2nd entry, 1 item
		//WheelEntry* l2 = new WheelEntry();
		//l2->_items.push_back(o2);

		//// 3rd entry in w1, contains 3 items
		//WheelEntry* l3 = new WheelEntry();
		//l3->_items.push_back(o3);
		//l3->_items.push_back(o4);
		//l3->_items.push_back(o5);

		//WheelEntry* l4 = new WheelEntry();
		//l4->_items.push_back(o6);

		//WheelEntry* l5 = new WheelEntry();
		//l5->_items.push_back(o7);
		//
		//w1->entries.push_back(l1);
		//w1->entries.push_back(l2);
		//w1->entries.push_back(l3);
		//w1->entries.push_back(l4);
		//w1->entries.push_back(l5);

		
		auto generateSpellWheelItem = [dh](RE::FormID formID) {
			return std::make_shared<WheelItemSpeel>((RE::SpellItem*)dh->LookupForm(formID, "Skyrim.esm"));
		};

		Wheeler::Wheel* w2 = new Wheeler::Wheel();
		
		WheelEntry* e6 = new WheelEntry();
		WheelEntry* e7 = new WheelEntry();
		WheelEntry* e8 = new WheelEntry();
		WheelEntry* e9 = new WheelEntry();
		WheelEntry* e10 = new WheelEntry();
		WheelEntry* e11 = new WheelEntry();
		e6->_items.push_back(generateSpellWheelItem(0x12FCC));  // healing
		e7->_items.push_back(generateSpellWheelItem(0x12FCD));  // flames
		e7->_items.push_back(generateSpellWheelItem(0x12FD0));  // firebolt
		e7->_items.push_back(generateSpellWheelItem(0x1C789));  // fireball

		e8->_items.push_back(generateSpellWheelItem(0x2B96b));  // frostbite
		e9->_items.push_back(generateSpellWheelItem(0x13018));  // wardlesser
		e10->_items.push_back(generateSpellWheelItem(0x204c3));  // conjure flame atronach
		e11->_items.push_back(generateSpellWheelItem(0x4deee));  // frenzy
		
		w2->entries.push_back(e6);
		w2->entries.push_back(e7);
		w2->entries.push_back(e8);
		w2->entries.push_back(e9);
		w2->entries.push_back(e10);
		w2->entries.push_back(e11);

		Wheeler::Wheel* w3 = new Wheeler::Wheel();
		
		_wheels.push_back(w1);
		_wheels.push_back(w2);
		_wheels.push_back(w3);

	}
	
}

void Wheeler::Update()
{
	std::shared_lock<std::shared_mutex> lock(_wheelDataLock);
	float deltaTime = ImGui::GetIO().DeltaTime;
	using namespace Config::Styling::Wheel;
	if (!RE::PlayerCharacter::GetSingleton() || !RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
		return;
	}
	// begin draw
	auto ui = RE::UI::GetSingleton();
	if (!ui) {
		return;
	}

	if (_state == WheelState::KClosed) { // should close
		if (ImGui::IsPopupOpen(_wheelWindowID)) {         // if it's open, close it
			ImGui::SetNextWindowPos(ImVec2(-100, -100));  // set the pop-up pos to be outside the screen space.
			ImGui::BeginPopup(_wheelWindowID);
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			//ImGui::GetIO().MouseDrawCursor = false;
			auto controlMap = RE::ControlMap::GetSingleton();
			if (controlMap) {
				controlMap->ignoreKeyboardMouse = false;
			}
			if (_editMode) {
				showEditModeVanillaMenus(ui);
			}
		}
		return;
	}
	// state is opened, opening, or closing, draw the wheel with different alphas.

	if (!ImGui::IsPopupOpen(_wheelWindowID)) {  // should open, but not opened yet
		//ImGui::GetIO().MouseDrawCursor = true;
		auto controlMap = RE::ControlMap::GetSingleton();
		if (controlMap) {
			controlMap->ignoreKeyboardMouse = true;
		}
		ImGui::OpenPopup(_wheelWindowID);
		_activeEntryIdx = -1; // reset active item on reopen
		_cursorPos = { 0, 0 }; // reset cursor pos
	}
	
	
	ImGui::SetNextWindowPos(ImVec2(-100, -100));  // set the pop-up pos to be outside the screen space.

	if (shouldBeInEditMode(ui)) {
		if (!_editMode) {
			enterEditMode();
		}
		hideEditModeVanillaMenus(ui);
	} else {
		if (_editMode) {
			exitEditMode();
		}
	}

	bool poppedUp = _editMode ? ImGui::BeginPopupModal(_wheelWindowID) : ImGui::BeginPopup(_wheelWindowID);
	if (poppedUp) {
		RE::TESObjectREFR::InventoryItemMap inv = RE::PlayerCharacter::GetSingleton()->GetInventory();

		const ImVec2 wheelCenter = getWheelCenter();

		auto drawList = ImGui::GetWindowDrawList();

		drawList->PushClipRectFullScreen();


		Wheel* wheel = _wheels[_activeWheelIdx];
		// draws the pie menu
		int numItems = wheel->entries.size();

		//ImGui::GetWindowDrawList()->AddCircle(wheelCenter, RADIUS_MIN, ImGui::GetColorU32(ImGuiCol_Border), 100, 2.0f);
		//ImGui::GetWindowDrawList()->AddCircle(wheelCenter, RADIUS_MAX, ImGui::GetColorU32(ImGuiCol_Border), 100, 2.0f);
		float cursorAngle = atan2f(_cursorPos.y, _cursorPos.x);

		if (numItems == 0) {
			Drawer::draw_text(wheelCenter.x, wheelCenter.y, 0, 0, "Empty Wheel", 255, 255, 255, 255, 40.f);
			// draw an arc so users don't get confused
			//drawList->PathArcTo(wheelCenter, (InnerCircleRadius + OuterCircleRadius) * 0.5f, 0.0f, IM_PI * 2.0f * 0.99f, 128);  // FIXME: 0.99f look like full arc with closed thick stroke has a bug now
			//drawList->PathStroke(BackGroundColor, true, OuterCircleRadius - InnerCircleRadius);
		}
		for (int entryIdx = 0; entryIdx < numItems; entryIdx++) {
			// fancy math begin

			const float entryArcSpan = 2 * IM_PI / ImMax(ITEMS_MIN, numItems);

			const float innerSpacing = InnerSpacing / InnerCircleRadius / 2;
			float entryInnerAngleMin = entryArcSpan * (entryIdx - 0.5f + innerSpacing) + IM_PI / 2;
			float entryInnerAngleMax = entryArcSpan * (entryIdx + 0.5f - innerSpacing) + IM_PI / 2;
			
			float entryOuterAngleMin = entryArcSpan * (entryIdx - 0.5f + innerSpacing * (InnerCircleRadius / OuterCircleRadius)) + IM_PI / 2;
			float entryOuterAngleMax = entryArcSpan * (entryIdx + 0.5f - innerSpacing * (InnerCircleRadius / OuterCircleRadius)) + IM_PI / 2;
			
			if (entryInnerAngleMax > IM_PI * 2) {
				entryInnerAngleMin -= IM_PI * 2;
				entryInnerAngleMax -= IM_PI * 2;
			}
			
			if (entryOuterAngleMax > IM_PI * 2) {
				entryOuterAngleMin -= IM_PI * 2;
				entryOuterAngleMax -= IM_PI * 2;
			}

			// update hovered item
			if (_cursorPos.x != 0 || _cursorPos.y != 0) {
				bool updatedActiveEntry = false;
				float cursorIndicatorToCenterDist = InnerCircleRadius - CursorIndicatorDist;
				Drawer::draw_arc(wheelCenter,
					cursorIndicatorToCenterDist - (CusorIndicatorArcWidth / 2),
					cursorIndicatorToCenterDist + (CusorIndicatorArcWidth / 2),
					cursorAngle - (CursorIndicatorArcAngle / 2), cursorAngle + (CursorIndicatorArcAngle / 2),
					cursorAngle - (CursorIndicatorArcAngle / 2), cursorAngle + (CursorIndicatorArcAngle / 2),
					CursorIndicatorColor,
					32);
				ImVec2 cursorIndicatorTriPts[3] = {
					{ cursorIndicatorToCenterDist + (CusorIndicatorArcWidth / 2), +CursorIndicatorTriangleSideLength },
					{ cursorIndicatorToCenterDist + (CusorIndicatorArcWidth / 2), -CursorIndicatorTriangleSideLength },
					{ cursorIndicatorToCenterDist + (CusorIndicatorArcWidth / 2) + CursorIndicatorTriangleSideLength, 0 }
				};
				for (ImVec2& pos: cursorIndicatorTriPts) {
					pos = ImRotate(pos, cos(cursorAngle), sin(cursorAngle));
				}
				Drawer::draw_triangle_filled(cursorIndicatorTriPts[0] + wheelCenter, cursorIndicatorTriPts[1] + wheelCenter, cursorIndicatorTriPts[2] + wheelCenter, CursorIndicatorColor);
				if (cursorAngle >= entryInnerAngleMin) {  // Normal case
					if (cursorAngle < entryInnerAngleMax) {
						if (entryIdx != _activeEntryIdx) {
							_activeEntryIdx = entryIdx;
							updatedActiveEntry = true;
						}
					}
				} else if (cursorAngle + 2 * IM_PI < entryInnerAngleMax && cursorAngle + 2 * IM_PI >= entryInnerAngleMin) {  // Wrap-around case
					if (entryIdx != _activeEntryIdx) {
						_activeEntryIdx = entryIdx;
						updatedActiveEntry = true;
					}
				}
				if (Config::Sound::EntrySwitchSound && updatedActiveEntry) {
					RE::PlaySoundRE(SD_ENTRYSWITCH);
				}
			}
			bool hovered = _activeEntryIdx == entryIdx;

			// calculate wheel center
			float t1 = (OuterCircleRadius - InnerCircleRadius) / 2;
			float t2 = InnerCircleRadius + t1;
			float rad = (entryInnerAngleMax - entryInnerAngleMin) / 2 + entryInnerAngleMin;
			float t3 = cosf(rad);
			float t4 = sinf(rad);
			ImVec2 itemCenter = ImVec2(
				wheelCenter.x + t2 * t3,
				wheelCenter.y + t2 * t4
				);
			
			int numArcSegments = (int)(256 * entryArcSpan / (2 * IM_PI)) + 1;
			// fancy math end
			Drawer::draw_arc_gradient(wheelCenter,
				InnerCircleRadius + InnerSpacing,
				OuterCircleRadius - InnerSpacing,
				entryInnerAngleMin, entryInnerAngleMax,
				entryOuterAngleMin, entryOuterAngleMax,
				hovered ? HoveredColorBegin : UnhoveredColorBegin,
				hovered ? HoveredColorEnd : UnhoveredColorEnd,
				numArcSegments);


			WheelEntry* entry = wheel->entries[entryIdx];

			bool active = entry->IsActive(inv);
			ImU32 arcColorBegin = active ? ActiveArcColorBegin : InActiveArcColorBegin;
			ImU32 arcColorEnd = active ? ActiveArcColorEnd : InActiveArcColorEnd;
			
			Drawer::draw_arc_gradient(wheelCenter,
				OuterCircleRadius - InnerSpacing,
				OuterCircleRadius - InnerSpacing + ActiveArcWidth,
				entryOuterAngleMin,
				entryOuterAngleMax,
				entryOuterAngleMin, 
				entryOuterAngleMax,
				arcColorBegin,
				arcColorEnd,
				numArcSegments);
			
			if (hovered) {
				entry->DrawHighlight(wheelCenter, inv);
			}
			entry->DrawSlot(itemCenter, hovered, inv);
		}
		// draw cursor indicator
		//drawList->AddCircleFilled(_cursorPos + wheelCenter, 10, ImGui::GetColorU32(ImGuiCol_Border), 10);
		// draw wheel indicator
		for (int i = 0; i < _wheels.size(); i++) {
			if (i == _activeWheelIdx) {
				Drawer::draw_circle_filled(
					{ wheelCenter.x + Config::Styling::Wheel::WheelIndicatorOffsetX + i * Config::Styling::Wheel::WheelIndicatorSpacing,
						wheelCenter.y + Config::Styling::Wheel::WheelIndicatorOffsetY },
					Config::Styling::Wheel::WheelIndicatorSize, Config::Styling::Wheel::WheelIndicatorActiveColor, 10);					
			} else {
				Drawer::draw_circle_filled(
					{ wheelCenter.x + Config::Styling::Wheel::WheelIndicatorOffsetX + i * Config::Styling::Wheel::WheelIndicatorSpacing,
						wheelCenter.y + Config::Styling::Wheel::WheelIndicatorOffsetY },
					Config::Styling::Wheel::WheelIndicatorSize, Config::Styling::Wheel::WheelIndicatorInactiveColor, 10);
			}
		}

		// update fade timer, alpha and wheel state.
		_openTimer += deltaTime;

		float alphaMult = 1.0f;
		switch (_state) {
		case WheelState::KOpening:
			if (_openTimer >= Config::Styling::Wheel::FadeTime) {
				_state = WheelState::KOpened;
			} else {
				alphaMult = _openTimer / Config::Styling::Wheel::FadeTime;
			}
			break;
		case WheelState::KClosing:
			_closeTimer += deltaTime;
			if (_closeTimer >= Config::Styling::Wheel::FadeTime) {
				CloseWheel();
				_closeTimer = 0;
			} else {
				alphaMult = 1 - _closeTimer / Config::Styling::Wheel::FadeTime;
			}
			break;
		}

		Drawer::set_alpha_mult(alphaMult);
		drawList->PopClipRect();
		ImGui::EndPopup();
	}

}

// TODO: swap to real serializer after implementing it
void Wheeler::LoadWheelItems()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);
	if (_state != WheelState::KClosed) { 
		CloseWheel(); // force close menu, since we're loading items
	}
	if (_editMode) {
		exitEditMode();
	}
	_activeEntryIdx = -1;
	WheelItemMutableManager::GetSingleton()->Clear();
	// clean up old wheels
	for (Wheel* wheel : _wheels) {
		for (WheelEntry* e : wheel->entries) {
			delete e;  // WheelEntry's destructor frees underlying items
		}
		delete wheel;
	}
	_wheels.clear();
	
	//Serializer::LoadData(_items);
	TestData::Load(_wheels);

	for (Wheel* wheel : _wheels) {
		//verifyWheelItems(wheel->entries);
	}
}

// TODO: implement this and the serializer
void Wheeler::FlushWheelItems()
{
	INFO("Flushing wheel items...");
	//Serializer::FlushData(_items);
	INFO("...wheel items flushed!");
}

void Wheeler::ToggleWheel()
{
	if (_state == WheelState::KClosed) {
		TryOpenWheel();
	} else {
		TryCloseWheel();
	}
}

void Wheeler::CloseWheelIfOpenedLongEnough()
{
	if (_state == WheelState::KOpened && _openTimer > PRESS_THRESHOLD) {
		TryCloseWheel();
	}
}

void Wheeler::TryOpenWheel()
{
	// here we straight up open the wheel, and set state to opening if we have a fade time.
	// this is because for the fade to start showing the wheel has to be actually fully opened,
	// but to track the state we give it a "opening" state.
	OpenWheel();
}

void Wheeler::TryCloseWheel()
{
	if (_state == WheelState::KClosed || _state == WheelState::KClosing) {
		return;
	}
	if (Config::Styling::Wheel::FadeTime == 0) {
		CloseWheel(); // close directly
	} else {
		// set timescale to 1 prior to closing the wheel to avoid weirdness
		if (Config::Styling::Wheel::SlowTimeScale <= 1) {
			if (Utils::Time::GGTM() != 1) {
				Utils::Time::SGTM(1);
			}
		}
		_state = WheelState::KClosing;  // set state to closing, will be closed once time out
		_closeTimer = 0;
	}
}

void Wheeler::OpenWheel()
{
	if (!RE::PlayerCharacter::GetSingleton() || !RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
		return;
	}
	if (_state != WheelState::KOpened && _state != WheelState::KOpening) {
		if (Config::Styling::Wheel::SlowTimeScale <= 1) {
			if (Utils::Time::GGTM() == 1) {
				Utils::Time::SGTM(Config::Styling::Wheel::SlowTimeScale);
			}
		}
		if (Config::Styling::Wheel::BlurOnOpen) {
			RE::UIBlurManager::GetSingleton()->IncrementBlurCount();
		}
		_state = Config::Styling::Wheel::FadeTime > 0 ? WheelState::KOpening : WheelState::KOpened;
		_openTimer = 0;
		RE::PlaySoundRE(SD_WHEELERTOGGLE);
	}
}

void Wheeler::CloseWheel()
{
	if (!RE::PlayerCharacter::GetSingleton() || !RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
		return;
	}
	if (_state != WheelState::KClosed) {
		if (Config::Styling::Wheel::SlowTimeScale <= 1) {
			if (Utils::Time::GGTM() != 1) {
				Utils::Time::SGTM(1);
			}
		}
		if (Config::Styling::Wheel::BlurOnOpen) {
			RE::UIBlurManager::GetSingleton()->DecrementBlurCount();
		}
		_openTimer = 0;
		_closeTimer = 0;
	}
	_state = WheelState::KClosed;
}

void Wheeler::UpdateCursorPosMouse(float a_deltaX, float a_deltaY)
{
	ImVec2 newPos = _cursorPos + ImVec2{ a_deltaX, a_deltaY };
	// Calculate the distance from the wheel center to the new cursor position
	float distanceFromCenter = sqrt(newPos.x * newPos.x + newPos.y * newPos.y);

	// If the distance exceeds the cursor radius, adjust the cursor position
	if (distanceFromCenter > Config::Control::Wheel::CursorRadiusPerEntry) {
		// Calculate the normalized direction vector from the center to the new position
		ImVec2 direction = newPos / distanceFromCenter;

		// Set the cursor position at the edge of the cursor radius
		newPos = direction * Config::Control::Wheel::CursorRadiusPerEntry;
	}
	
	_cursorPos = newPos;
}

void Wheeler::UpdateCursorPosGamepad(float a_x, float a_y)
{
	ImVec2 newPos = { a_x,a_y };
	float distanceFromCenter = sqrt(newPos.x * newPos.x + newPos.y * newPos.y);
	// If the distance exceeds the cursor radius, adjust the cursor position
	if (distanceFromCenter > Config::Control::Wheel::CursorRadiusPerEntry) {
		// Calculate the normalized direction vector from the center to the new position
		ImVec2 direction = newPos / distanceFromCenter;

		// Set the cursor position at the edge of the cursor radius
		newPos = direction * Config::Control::Wheel::CursorRadiusPerEntry;
	}

	_cursorPos = newPos;
}


void Wheeler::NextWheel()
{
	if (_state == WheelState::KOpened) {
		_cursorPos = { 0, 0 };
		_activeEntryIdx = -1;
		_activeWheelIdx += 1;
		if (_activeWheelIdx >= _wheels.size()) {
			_activeWheelIdx = 0;
		}
		if (Config::Sound::WheelSwitchSound && _wheels.size() > 1) {
			RE::PlaySoundRE(SD_WHEELSWITCH);
		}
	}
}

void Wheeler::PrevWheel()
{
	if (_state == WheelState::KOpened) {
		_cursorPos = { 0, 0 };
		_activeEntryIdx = -1;
		_activeWheelIdx -= 1;
		if (_activeWheelIdx < 0) {
			_activeWheelIdx = _wheels.size() - 1;
		}
		if (Config::Sound::WheelSwitchSound && _wheels.size() > 1) {
			RE::PlaySoundRE(SD_WHEELSWITCH);
		}
	}
}

void Wheeler::PrevItem()
{
	if (_state == WheelState::KOpened && _activeEntryIdx != -1) {
		_wheels[_activeWheelIdx]->entries[_activeEntryIdx]->PrevItem();
	}
}

void Wheeler::NextItem()
{
	if (_state == WheelState::KOpened && _activeEntryIdx != -1) {
		_wheels[_activeWheelIdx]->entries[_activeEntryIdx]->NextItem();
	}
}

void Wheeler::ActivateEntryLeft()
{
	if (_state == WheelState::KOpened) {
		Wheel* activeWheel = _wheels[_activeWheelIdx];
		if (activeWheel->entries.empty()) {  // empty wheel, we can only delete in edit mode.
			if (_editMode && _wheels.size() > 1) {  // we have more than one wheel, so it's safe to delete this one.
				DeleteCurrentWheel();
			}
		} else if (_activeEntryIdx != -1) {
			WheelEntry* activeEntry = _wheels[_activeWheelIdx]->entries[_activeEntryIdx];
			if (_editMode && activeEntry->IsEmpty()) {
				DeleteCurrentEntry(); // delete empty entry
				return;
			}
			activeEntry->ActivateItemLeft(_editMode);  // activeEntry will handle deleting the entry's items
		}
	}
}

void Wheeler::ActivateEntryRight()
{
	if (_state == WheelState::KOpened && _activeEntryIdx != -1 && !_wheels[_activeWheelIdx]->entries.empty()) {
		_wheels[_activeWheelIdx]->entries[_activeEntryIdx]->ActivateItemRight(_editMode);
	}
}

void Wheeler::AddEntryToCurrentWheel()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);
	if (!_editMode) {
		return;
	}
	WheelEntry* newEntry = new WheelEntry();
	Wheel* activeWheel = _wheels[_activeWheelIdx];
	//activeWheel->entries.insert(activeWheel->entries.begin() + activeWheel->entries.size() - 1, newEntry); for deprecated WheelItemAdder impl
	activeWheel->entries.push_back(newEntry);
}

void Wheeler::DeleteCurrentEntry()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);

	if (!_editMode) {
		return;
	}
	Wheel* activeWheel = _wheels[_activeWheelIdx];

	WheelEntry* toDelete = activeWheel->entries[_activeEntryIdx];
	activeWheel->entries.erase(activeWheel->entries.begin() + _activeEntryIdx);
	delete toDelete;
	if (_activeEntryIdx == activeWheel->entries.size() && _activeEntryIdx != 0) {  //deleted the last entry
		_activeEntryIdx = activeWheel->entries.size() - 1;
	}
	
}

void Wheeler::AddWheel()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);
	if (!_editMode) {
		return;
	}
	Wheel* newWheel = new Wheel();
	_wheels.push_back(newWheel);
}

void Wheeler::DeleteCurrentWheel()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);
	if (!_editMode) {
		return;
	}
	if (_wheels.size() > 1) {
		Wheel* toDelete = _wheels[_activeWheelIdx];
		if (toDelete->entries.size() > 1) {
			throw std::runtime_error("Cannot delete wheel with more than one entry!");
		}
		_wheels.erase(_wheels.begin() + _activeWheelIdx);
		delete toDelete;
		if (_activeWheelIdx == _wheels.size() && _activeWheelIdx != 0) {  //deleted the last wheel
			_activeWheelIdx = _wheels.size() - 1;
		}
		_activeEntryIdx = -1; // no entry is active
	}
}

void Wheeler::MoveEntryForward()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);
	if (!_editMode) {
		return;
	}
	if (_activeEntryIdx != -1 && _wheels[_activeWheelIdx]->entries.size() > 1) {
		Wheel* activeWheel = _wheels[_activeWheelIdx];
		int targetIdx = _activeEntryIdx + 1 >= activeWheel->entries.size() ? 0 : _activeEntryIdx + 1;
		std::swap(activeWheel->entries[_activeEntryIdx], activeWheel->entries[targetIdx]);
		_activeEntryIdx = targetIdx;
	}
}

void Wheeler::MoveEntryBack()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);
	if (!_editMode) {
		return;
	}
	if (_activeEntryIdx != -1 && _wheels[_activeWheelIdx]->entries.size() > 1) {
		Wheel* activeWheel = _wheels[_activeWheelIdx];
		int targetIdx = _activeEntryIdx - 1 < 0 ? activeWheel->entries.size() - 1 : _activeEntryIdx - 1;
		std::swap(activeWheel->entries[_activeEntryIdx], activeWheel->entries[targetIdx]);
		_activeEntryIdx = targetIdx;
	}
}

void Wheeler::MoveWheelForward()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);
	if (!_editMode) {
		return;
	}
	if (_wheels.size() > 1) {
		if (_activeWheelIdx == _wheels.size() - 1) {
			// Move the current wheel to the very front
			auto currentWheel = std::move(_wheels.back());
			_wheels.pop_back();
			_wheels.insert(_wheels.begin(), std::move(currentWheel));
		} else {
			int targetIdx = _activeWheelIdx + 1;
			std::swap(_wheels[_activeWheelIdx], _wheels[targetIdx]);
			_activeWheelIdx = targetIdx;
		}
	}
}


void Wheeler::MoveWheelBack()
{
	std::unique_lock<std::shared_mutex> lock(_wheelDataLock);
	if (!_editMode) {
		return;
	}
	if (_wheels.size() > 1) {
		if (_activeWheelIdx == 0) {
			// Move the current wheel to the very end
			auto currentWheel = std::move(_wheels.front());
			_wheels.erase(_wheels.begin());
			_wheels.push_back(std::move(currentWheel));
			_activeWheelIdx = _wheels.size() - 1;
		} else {
			int targetIdx = _activeWheelIdx - 1;
			std::swap(_wheels[_activeWheelIdx], _wheels[targetIdx]);
			_activeWheelIdx = targetIdx;
		}
	}
}



inline ImVec2 Wheeler::getWheelCenter()
{
	return ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
}

bool Wheeler::shouldBeInEditMode(RE::UI* a_ui)
{
	return a_ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME) || a_ui->IsMenuOpen(RE::MagicMenu::MENU_NAME);
}

void Wheeler::hideEditModeVanillaMenus(RE::UI* a_ui)
{
	if (a_ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME)) {
		RE::GFxMovieView* uiMovie = a_ui->GetMenu<RE::InventoryMenu>()->uiMovie.get();
		if (uiMovie) {
			uiMovie->SetVisible(false);
		}
	}
	if (a_ui->IsMenuOpen(RE::MagicMenu::MENU_NAME)) {
		RE::GFxMovieView* uiMovie = a_ui->GetMenu<RE::MagicMenu>()->uiMovie.get();
		if (uiMovie) {
			uiMovie->SetVisible(false);
		}
	}
}

void Wheeler::showEditModeVanillaMenus(RE::UI* a_ui)
{
	if (a_ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME)) {
		RE::GFxMovieView* uiMovie = a_ui->GetMenu<RE::InventoryMenu>()->uiMovie.get();
		if (uiMovie) {
			uiMovie->SetVisible(true);
		}
	}
	if (a_ui->IsMenuOpen(RE::MagicMenu::MENU_NAME)) {
		RE::GFxMovieView* uiMovie = a_ui->GetMenu<RE::MagicMenu>()->uiMovie.get();
		if (uiMovie) {
			uiMovie->SetVisible(true);
		}
	}
}

void Wheeler::enterEditMode()
{
	if (_editMode) {
		return;
	}
	//for (auto wheel : _wheels) {
	//	wheel->entries.push_back(WheelEntryAdder::GetSingleton());
	//}
	
	_editMode = true;
}

void Wheeler::exitEditMode()
{
	if (!_editMode) {
		return;
	}
	/*for (auto wheel : _wheels) {
		wheel->entries.pop_back();
	}*/
	
	_editMode = false;
}


