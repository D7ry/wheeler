#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Wheeler.h"
#include "Serializer.h"

#include "Input.h"
#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_internal.h"
#include "imgui.h"
#include "WheelItems/WheelItem.h"

#include "WheelItems/WheelItemWeapon.h"
#include "include/lib/Drawer.h"
#include "Controls.h"
namespace TestData
{
	void LoadTestItems1(std::vector<WheelItem*>& r_wheelItems)
	{
		auto dh = RE::TESDataHandler::GetSingleton();
		
		auto w1 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139A4, "Skyrim.esm")); // glass battle axe
		auto w2 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139B6, "Skyrim.esm")); // daedric dagger
		auto w3 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139B1, "Skyrim.esm")); // ebony sword

		r_wheelItems.push_back(w1);
		r_wheelItems.push_back(w2);
		r_wheelItems.push_back(w3);
	}

	void LoadTestItems2(std::vector<WheelItem*>& r_wheelItems)
	{
		auto dh = RE::TESDataHandler::GetSingleton();

		auto w4 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x13987, "Skyrim.esm")); // steel greatsword
		auto w5 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0xA5DEF, "Skyrim.esm")); // Gauldur Blackbow

		r_wheelItems.push_back(w4);
		r_wheelItems.push_back(w5);
	}
	
}

void Wheeler::Draw()
{
	using namespace Config::Styling::Wheel;

	if (!_active) { // queued to close
		if (ImGui::IsPopupOpen(_wheelWindowID)) {
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
	
	ImGui::SetNextWindowPos(ImVec2(-100, -100));  // set the pop-up pos to be outside the screen space.
	// begin draw
	if (ImGui::BeginPopup(_wheelWindowID)) {
		const ImVec2 wheelCenter = getWheelCenter();

		auto drawList = ImGui::GetWindowDrawList();

		drawList->PushClipRectFullScreen();
		drawList->PathArcTo(wheelCenter, (RADIUS_MIN + RADIUS_MAX) * 0.5f, 0.0f, IM_PI * 2.0f * 0.99f, 128);  // FIXME: 0.99f look like full arc with closed thick stroke has a bug now
		drawList->PathStroke(BackGroundColor, true, RADIUS_MAX - RADIUS_MIN);

		Wheel* wheel = _wheels[_activeWheel];
		// draws the pie menu
		int numItems = wheel->items.size();

		//ImGui::GetWindowDrawList()->AddCircle(wheelCenter, RADIUS_MIN, ImGui::GetColorU32(ImGuiCol_Border), 100, 2.0f);
		//ImGui::GetWindowDrawList()->AddCircle(wheelCenter, RADIUS_MAX, ImGui::GetColorU32(ImGuiCol_Border), 100, 2.0f);
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

				float drag_angle = atan2f(_cursorPos.y, _cursorPos.x);

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
			
			int arc_segments = (int)(64 * item_arc_span / (2 * IM_PI)) + 1;
			// fancy math end
			Drawer::draw_arc(wheelCenter,
				RADIUS_MIN + ITEM_INNER_SPACING,
				RADIUS_MAX - ITEM_INNER_SPACING,
				item_inner_ang_min, item_inner_ang_max,
				item_outer_ang_min, item_outer_ang_max,
				hovered ? HoveredColor : UnhoveredColor, arc_segments);

			drawList->AddCircleFilled(_cursorPos + wheelCenter, 10, ImGui::GetColorU32(ImGuiCol_Border), 10);

			WheelItem* item = wheel->items[item_n];

			if (item->IsActive() || hovered) {
				Drawer::draw_arc(wheelCenter,
					RADIUS_MAX - ITEM_INNER_SPACING,
					RADIUS_MAX - ITEM_INNER_SPACING + ActiveArcWidth,
					item_outer_ang_min, item_outer_ang_max,
					item_outer_ang_min, item_outer_ang_max,
					ActiveArcColor, arc_segments);
			}
			
			if (hovered) {
				item->DrawHighlight(wheelCenter);
				for (uint32_t keyID : Controls::GetPressedKeys()) {
					item->ReceiveInput(keyID);
				}
			}
			item->DrawSlot(itemCenter, hovered);
		}
		Controls::FlushPressedKeys();
		drawList->PopClipRect();
		ImGui::EndPopup();
	}

}

// TODO: swap to real serializer after implementing it
void Wheeler::LoadWheelItems()
{
	_active = false;
	_activeItem = -1;

	int i = 0;
	for (Wheel* wheel : _wheels) {
		for (WheelItem* item : wheel->items) {
			delete item;  // delete old items
		}
		wheel->items.clear();
		
		if (i == 0) {
			TestData::LoadTestItems1(wheel->items);
		} else if (i == 1) {
			TestData::LoadTestItems2(wheel->items);
		}
		//Serializer::LoadData(_items); 
		verifyWheelItems(wheel->items);
		i++;
	}
}

// TODO: implement this and the serializer
void Wheeler::FlushWheelItems()
{
	INFO("Flushing wheel items...");
	//Serializer::FlushData(_items);
	INFO("...wheel items flushed!");
}

void Wheeler::OpenMenu()
{
	_active = true;
}

void Wheeler::CloseMenu()
{
	_active = false;
}

void Wheeler::ToggleEditMode()
{
	_editMode = _editMode;
}

void Wheeler::verifyWheelItems(std::vector<WheelItem*> a_items)
{
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
	_activeItem = -1;
	_activeWheel = (_activeWheel + 1) % _wheels.size();
}

void Wheeler::PrevWheel()
{
	_activeItem = -1;
	_activeWheel = (_activeWheel - 1) % _wheels.size();
}


inline ImVec2 Wheeler::getWheelCenter()
{
	return ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
}

