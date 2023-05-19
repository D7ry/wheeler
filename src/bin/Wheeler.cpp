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
			return new WheelItemSpeel((RE::SpellItem*)dh->LookupForm(formID, "Skyrim.esm"));
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

void Wheeler::Draw()
{
	std::lock_guard<std::mutex> lock(_wheelDataLock);

	using namespace Config::Styling::Wheel;
	if (!RE::PlayerCharacter::GetSingleton() || !RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
		return;
	}

	if (!_active) { // queued to close
		if (ImGui::IsPopupOpen(_wheelWindowID)) {
			ImGui::SetNextWindowPos(ImVec2(-100, -100));  // set the pop-up pos to be outside the screen space.
			ImGui::BeginPopup(_wheelWindowID);
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			//ImGui::GetIO().MouseDrawCursor = false;
			auto controlMap = RE::ControlMap::GetSingleton();
			if (controlMap) {
				controlMap->ignoreKeyboardMouse = false;
			}
		}
		return;
	}
	if (!ImGui::IsPopupOpen(_wheelWindowID)) {  // queued to open
		//ImGui::GetIO().MouseDrawCursor = true;
		auto controlMap = RE::ControlMap::GetSingleton();
		if (controlMap) {
			controlMap->ignoreKeyboardMouse = true;
		}
		ImGui::OpenPopup(_wheelWindowID);
		_activeItem = -1; // reset active item on reopen
		_cursorPos = { 0, 0 }; // reset cursor pos
	}
	_openTimer += ImGui::GetIO().DeltaTime;
	
	
	ImGui::SetNextWindowPos(ImVec2(-100, -100));  // set the pop-up pos to be outside the screen space.
	// begin draw
	if (RE::UI::GetSingleton()->IsMenuOpen(RE::InventoryMenu::MENU_NAME)) {
		if (!_editMode) {
			enterEditMode();
		}
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
		//drawList->PathArcTo(wheelCenter, (RADIUS_MIN + RADIUS_MAX) * 0.5f, 0.0f, IM_PI * 2.0f * 0.99f, 128);  // FIXME: 0.99f look like full arc with closed thick stroke has a bug now
		//drawList->PathStroke(BackGroundColor, true, RADIUS_MAX - RADIUS_MIN);

		Wheel* wheel = _wheels[_activeWheel];
		// draws the pie menu
		int numItems = wheel->entries.size();

		//ImGui::GetWindowDrawList()->AddCircle(wheelCenter, RADIUS_MIN, ImGui::GetColorU32(ImGuiCol_Border), 100, 2.0f);
		//ImGui::GetWindowDrawList()->AddCircle(wheelCenter, RADIUS_MAX, ImGui::GetColorU32(ImGuiCol_Border), 100, 2.0f);
		float drag_angle = atan2f(_cursorPos.y, _cursorPos.x);


		for (int item_n = 0; item_n < numItems; item_n++) {
			// fancy math begin

			const float item_arc_span = 2 * IM_PI / ImMax(ITEMS_MIN, numItems);

			const float inner_spacing = ITEM_INNER_SPACING / RADIUS_MIN / 2;
			float item_inner_ang_min = item_arc_span * (item_n - 0.5f + inner_spacing) + IM_PI / 2;
			float item_inner_ang_max = item_arc_span * (item_n + 0.5f - inner_spacing) + IM_PI / 2;
			
			float item_outer_ang_min = item_arc_span * (item_n - 0.5f + inner_spacing * (RADIUS_MIN / RADIUS_MAX)) + IM_PI / 2;
			float item_outer_ang_max = item_arc_span * (item_n + 0.5f - inner_spacing * (RADIUS_MIN / RADIUS_MAX)) + IM_PI / 2;
			
			if (item_inner_ang_max > IM_PI * 2) {
				item_inner_ang_min -= IM_PI * 2;
				item_inner_ang_max -= IM_PI * 2;
			}
			
			if (item_outer_ang_max > IM_PI * 2) {
				item_outer_ang_min -= IM_PI * 2;
				item_outer_ang_max -= IM_PI * 2;
			}

			// update hovered item
			if (_cursorPos.x != 0 || _cursorPos.y != 0) {
				if (drag_angle >= item_inner_ang_min) {  // Normal case
					if (drag_angle < item_inner_ang_max) {
						_activeItem = item_n;
					}
				} else if (drag_angle + 2 * IM_PI < item_inner_ang_max && drag_angle + 2 * IM_PI >= item_inner_ang_min) {  // Wrap-around case
					_activeItem = item_n;
				}
			}
			bool hovered = _activeItem == item_n;

			// calculate wheel center
			float t1 = (RADIUS_MAX - RADIUS_MIN) / 2;
			float t2 = RADIUS_MIN + t1;
			float rad = (item_inner_ang_max - item_inner_ang_min) / 2 + item_inner_ang_min;
			float t3 = cosf(rad);
			float t4 = sinf(rad);
			ImVec2 itemCenter = ImVec2(
				wheelCenter.x + t2 * t3,
				wheelCenter.y + t2 * t4
				);
			
			int arc_segments = (int)(256 * item_arc_span / (2 * IM_PI)) + 1;
			// fancy math end
			Drawer::draw_arc_gradient(wheelCenter,
				RADIUS_MIN + ITEM_INNER_SPACING,
				RADIUS_MAX - ITEM_INNER_SPACING,
				item_inner_ang_min, item_inner_ang_max,
				item_outer_ang_min, item_outer_ang_max,
				hovered ? HoveredColorBegin : UnhoveredColorBegin,
				hovered ? HoveredColorEnd : UnhoveredColorEnd,
				arc_segments);


			WheelEntry* entry = wheel->entries[item_n];

			bool active = entry->IsActive(inv);
			Drawer::draw_arc_gradient(wheelCenter,
				RADIUS_MAX - ITEM_INNER_SPACING,
				RADIUS_MAX - ITEM_INNER_SPACING + ActiveArcWidth,
				item_outer_ang_min, item_outer_ang_max,
				item_outer_ang_min, item_outer_ang_max,
				active ? ActiveArcColorBegin : InActiveArcColorBegin,
				active ? ActiveArcColorEnd : InActiveArcColorEnd,
				arc_segments);
			
			if (hovered) {
				entry->DrawHighlight(wheelCenter, inv);
			}
			entry->DrawSlot(itemCenter, hovered, inv);
		}
		// draw cursor indicator
		drawList->AddCircleFilled(_cursorPos + wheelCenter, 10, ImGui::GetColorU32(ImGuiCol_Border), 10);
		// draw wheel indicator
		for (int i = 0; i < _wheels.size(); i++) {
			if (i == _activeWheel) {
				drawList->AddCircleFilled(
					{wheelCenter.x + Config::Styling::Wheel::WheelIndicatorOffsetX + i * Config::Styling::Wheel::WheelIndicatorSpacing,
					wheelCenter.y + Config::Styling::Wheel::WheelIndicatorOffsetY},
					Config::Styling::Wheel::WheelIndicatorSize, Config::Styling::Wheel::WheelIndicatorActiveColor, 10);
			} else {
				drawList->AddCircleFilled(
					
					
					{ wheelCenter.x + Config::Styling::Wheel::WheelIndicatorOffsetX + i * Config::Styling::Wheel::WheelIndicatorSpacing,
						wheelCenter.y + Config::Styling::Wheel::WheelIndicatorOffsetY },
					Config::Styling::Wheel::WheelIndicatorSize, Config::Styling::Wheel::WheelIndicatorInactiveColor, 10);
			}
		}
		
		drawList->PopClipRect();
		ImGui::EndPopup();
	}

}

// TODO: swap to real serializer after implementing it
void Wheeler::LoadWheelItems()
{
	std::lock_guard<std::mutex> lock(_wheelDataLock);
	if (_active) {
		CloseMenu();
	}
	if (_editMode) {
		exitEditMode();
	}
	_activeItem = -1;
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

void Wheeler::ToggleMenu()
{
	if (!_active) {
		OpenMenu();
	} else {
		CloseMenu();
	}
}

void Wheeler::CloseMenuIfOpenedLongEnough()
{
	if (_active && _openTimer > _pressThreshold) {
		CloseMenu();
	}
}

void Wheeler::OpenMenu()
{
	if (!RE::PlayerCharacter::GetSingleton() || !RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
		return;
	}
	if (!_active) {
		if (Config::Styling::Wheel::SlowTimeScale <= 1) {
			if (Utils::Time::GGTM() == 1) {
				Utils::Time::SGTM(Config::Styling::Wheel::SlowTimeScale);
			}
		}
		if (Config::Styling::Wheel::BlurOnOpen) {
			RE::UIBlurManager::GetSingleton()->IncrementBlurCount();
		}
		_active = true;
		_openTimer = 0;
	}
}

void Wheeler::CloseMenu()
{
	if (!RE::PlayerCharacter::GetSingleton() || !RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
		return;
	}
	if (_active) {
		if (Config::Styling::Wheel::SlowTimeScale <= 1) {
			if (Utils::Time::GGTM() != 1) {
				Utils::Time::SGTM(1);
			}
		}
		if (Config::Styling::Wheel::BlurOnOpen) {
			RE::UIBlurManager::GetSingleton()->DecrementBlurCount();
		}
		_active = false;
		_openTimer = 0;
	}
}

void Wheeler::UpdateCursorPosMouse(float a_deltaX, float a_deltaY)
{
	ImVec2 newPos = _cursorPos + ImVec2{ a_deltaX, a_deltaY };
	// Calculate the distance from the wheel center to the new cursor position
	float distanceFromCenter = sqrt(newPos.x * newPos.x + newPos.y * newPos.y);

	// If the distance exceeds the cursor radius, adjust the cursor position
	if (distanceFromCenter > Config::Control::Wheel::cursorRadius) {
		// Calculate the normalized direction vector from the center to the new position
		ImVec2 direction = newPos / distanceFromCenter;

		// Set the cursor position at the edge of the cursor radius
		newPos = direction * Config::Control::Wheel::cursorRadius;
	}
	
	_cursorPos = newPos;
}

void Wheeler::UpdateCursorPosGamepad(float a_x, float a_y)
{
	ImVec2 newPos = { a_x,a_y };
	float distanceFromCenter = sqrt(newPos.x * newPos.x + newPos.y * newPos.y);
	// If the distance exceeds the cursor radius, adjust the cursor position
	if (distanceFromCenter > Config::Control::Wheel::cursorRadius) {
		// Calculate the normalized direction vector from the center to the new position
		ImVec2 direction = newPos / distanceFromCenter;

		// Set the cursor position at the edge of the cursor radius
		newPos = direction * Config::Control::Wheel::cursorRadius;
	}

	_cursorPos = newPos;
}


void Wheeler::NextWheel()
{
	if (_active) {
		_cursorPos = { 0, 0 };
		_activeItem = -1;
		_activeWheel += 1;
		if (_activeWheel >= _wheels.size()) {
			_activeWheel = 0;
		}
	}
}

void Wheeler::PrevWheel()
{
	if (_active) {
		_cursorPos = { 0, 0 };
		_activeItem = -1;
		_activeWheel -= 1;
		if (_activeWheel < 0) {
			_activeWheel = _wheels.size() - 1;
		}
	}
}

void Wheeler::PrevItem()
{
	if (_active && _activeItem != -1) {
		_wheels[_activeWheel]->entries[_activeItem]->PrevItem();
	}
}

void Wheeler::NextItem()
{
	if (_active && _activeItem != -1) {
		_wheels[_activeWheel]->entries[_activeItem]->NextItem();
	}
}

void Wheeler::ActivateItemLeft()
{
	if (_active && _activeItem != -1) {
		_wheels[_activeWheel]->entries[_activeItem]->ActivateItemLeft(_editMode);
	}
}

void Wheeler::ActivateItemRight()
{
	if (_active && _activeItem != -1) {
		_wheels[_activeWheel]->entries[_activeItem]->ActivateItemRight(_editMode);
	}
}

void Wheeler::AddEntryToCurrentWheel()
{
	std::lock_guard<std::mutex> lock(_wheelDataLock);
	if (!_editMode) {
		throw std::runtime_error("AddEntryToCurrentWheel must be called in edit mode!");
	}
	WheelEntry* newEntry = new WheelEntry();
	Wheel* activeWheel = _wheels[_activeWheel];
	activeWheel->entries.insert(activeWheel->entries.begin() + activeWheel->entries.size() - 1, newEntry);
}


inline ImVec2 Wheeler::getWheelCenter()
{
	return ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
}

void Wheeler::enterEditMode()
{
	if (_editMode) {
		return;
	}
	for (auto wheel : _wheels) {
		wheel->entries.push_back(WheelEntryAdder::GetSingleton());
	}
	_editMode = true;
}

void Wheeler::exitEditMode()
{
	if (!_editMode) {
		return;
	}
	for (auto wheel : _wheels) {
		wheel->entries.pop_back();
	}
	_editMode = false;
}


