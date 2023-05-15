#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Wheeler.h"
#include "Serializer.h"

#include "Input.h"

#include "imgui_internal.h"
#include "imgui.h"

#include "WheelItems/WheelItemWeapon.h"
namespace TestData
{
	void LoadTestItems(std::vector<WheelItem*>& r_wheelItems)
	{
		//r_wheelItems.push_back(WheelItem());
		auto dh = RE::TESDataHandler::GetSingleton();
		
		auto w1 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139A4, "Skyrim.esm")); // glass battle axe
		auto w2 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139B6, "Skyrim.esm")); // daedric dagger
		auto w3 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x139B1, "Skyrim.esm")); // ebony sword
		auto w4 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0x13987, "Skyrim.esm")); // steel greatsword
		auto w5 = new WheelItemWeapon((RE::TESObjectWEAP*)dh->LookupForm(0xA5DEF, "Skyrim.esm")); // Gauldur Blackbow

		r_wheelItems.push_back(w1);
		r_wheelItems.push_back(w2);
		r_wheelItems.push_back(w3);
		r_wheelItems.push_back(w4);
		r_wheelItems.push_back(w5);
	}
}

void Wheeler::Draw()
{
	using namespace WheelerStyling;

	if (!this->_active) { // queued to close
		if (ImGui::IsPopupOpen(_wheelWindowID)) {
			ImGui::BeginPopup(_wheelWindowID);
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			ImGui::GetIO().MouseDrawCursor = false;
			auto controlMap = RE::ControlMap::GetSingleton();
			if (controlMap) {
				controlMap->ignoreKeyboardMouse = false;
			}
		}
		return;
	}
	if (!ImGui::IsPopupOpen(_wheelWindowID)) {  // queued to open
		ImGui::GetIO().MouseDrawCursor = true;
		auto controlMap = RE::ControlMap::GetSingleton();
		if (controlMap) {
			controlMap->ignoreKeyboardMouse = true;
		}
		ImGui::OpenPopup(_wheelWindowID);
	}

	ImGui::SetNextWindowPos(ImVec2(-100, -100));  // set the pop-up pos to be outside the screen space.
	// begin draw
	if (ImGui::BeginPopup(_wheelWindowID)) {
		const ImVec2 wheelCenter = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);

		auto drawList = ImGui::GetWindowDrawList();

		drawList->PushClipRectFullScreen();
		drawList->PathArcTo(wheelCenter, (RADIUS_MIN + RADIUS_MAX) * 0.5f, 0.0f, IM_PI * 2.0f * 0.99f, 32);  // FIXME: 0.99f look like full arc with closed thick stroke has a bug now
		drawList->PathStroke(ImColor(1.0f, 1.0f, 1.0f, 0.3f), true, RADIUS_MAX - RADIUS_MIN);
		// draws the pie menu
		int numItems = _items.size();
		
		//ImGui::GetWindowDrawList()->AddCircle(wheelCenter, RADIUS_MIN, ImGui::GetColorU32(ImGuiCol_Border), 100, 2.0f);
		//ImGui::GetWindowDrawList()->AddCircle(wheelCenter, RADIUS_MAX, ImGui::GetColorU32(ImGuiCol_Border), 100, 2.0f);
		for (int item_n = 0; item_n < numItems; item_n++) {
			// fancy math begin
			const ImVec2 drag_delta = ImVec2(ImGui::GetIO().MousePos.x - wheelCenter.x, ImGui::GetIO().MousePos.y - wheelCenter.y);
			const float drag_dist2 = drag_delta.x * drag_delta.x + drag_delta.y * drag_delta.y;

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

			float drag_angle = atan2f(drag_delta.y, drag_delta.x);

			bool hovered = false;
			if (drag_dist2 >= RADIUS_MIN * RADIUS_MIN) {
				if (drag_angle >= item_inner_ang_min) {  // Normal case
					if (drag_angle < item_inner_ang_max) {
						hovered = true;
					}
				} 
				else if (drag_angle + 2 * IM_PI < item_inner_ang_max && drag_angle + 2 * IM_PI >= item_inner_ang_min) {  // Wrap-around case
					hovered = true;
				}
			}


			// Calculate the starting angle adjustment for odd number of items
			//float start_angle_adjustment = (numItems % 2 != 0) ? -item_arc_span / 2.0f : 0.0f;

			float t1 = (RADIUS_MAX - RADIUS_MIN) / 2;
			float t2 = RADIUS_MIN + t1;
			float rad = (item_inner_ang_max - item_inner_ang_min) / 2 + item_inner_ang_min;
			float t3 = cosf(rad);
			float t4 = sinf(rad);
			ImVec2 itemCenter = ImVec2(
				wheelCenter.x + t2 * t3,
				wheelCenter.y + t2 * t4
				);
			
			WheelItem* item = _items[item_n];

			// debug
			//ImGui::GetWindowDrawList()->AddCircleFilled(itemCenter, 5, ImGui::GetColorU32(ImGuiCol_Button), 9);

			int arc_segments = (int)(128 * item_arc_span / (2 * IM_PI)) + 1;
			drawList->PathArcTo(wheelCenter, RADIUS_MAX - ITEM_INNER_SPACING, item_outer_ang_min, item_outer_ang_max, arc_segments);
			
			drawList->PathArcTo(wheelCenter, RADIUS_MIN + ITEM_INNER_SPACING, item_inner_ang_max, item_inner_ang_min, arc_segments);

			drawList->PathFillConvex(hovered ? ImColor(100, 100, 150, 100) : ImColor(70, 70, 70, 100));
			// fancy math end
			if (hovered) {
				for (uint32_t keyID : Input::GetSingleton()->GetPressedKeys()) {
					item->Activate(keyID);
				}
			}
			item->Draw(itemCenter, hovered);
		}
		drawList->PopClipRect();
		ImGui::EndPopup();
	}

}

// TODO: swap to real serializer after implementing it
void Wheeler::LoadWheelItems()
{
	std::vector<WheelItem*> cpy = this->_items;
	this->_items.clear();
	INFO("Loading wheel items...");
	TestData::LoadTestItems(this->_items);
	//Serializer::LoadData(_items); 
	INFO("...wheel items loaded!");
	this->verifyWheelItems(); // chip away invalid items
	for (WheelItem* item : cpy) {
		delete item;
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
	this->_active = true;
}

void Wheeler::CloseMenu()
{
	this->_active = false;
}

void Wheeler::ToggleEditMode()
{
	this->_editMode = !this->_editMode;
}

void Wheeler::verifyWheelItems()
{
}

