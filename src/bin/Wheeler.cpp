#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Wheeler.h"
#include "Serializer.h"

#include "Input.h"

#include "imgui_internal.h"

#include "WheelItems/WheelItemWeapon.h"
namespace TestData
{
	void LoadTestItems(std::vector<WheelItem*>& r_wheelItems)
	{
		//r_wheelItems.push_back(WheelItem());
		r_wheelItems.emplace_back(new WheelItemWeapon());
		r_wheelItems.emplace_back(new WheelItemWeapon());
		r_wheelItems.emplace_back(new WheelItemWeapon());
		r_wheelItems.emplace_back(new WheelItemWeapon());
		r_wheelItems.emplace_back(new WheelItemWeapon());
		r_wheelItems.emplace_back(new WheelItemWeapon());
	}
}

void Wheeler::Draw()
{
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
	
	const ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);

	ImGui::SetNextWindowPos(center);

	// begin draw
	if (ImGui::BeginPopup(_wheelWindowID)) {
		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushClipRectFullScreen();
		// draws the pie menu
		int numItems = _items.size();
		for (int item_n = 0; item_n < numItems; item_n++) {
			// fancy math begin
			const ImVec2 drag_delta = ImVec2(ImGui::GetIO().MousePos.x - center.x, ImGui::GetIO().MousePos.y - center.y);
			const float drag_dist2 = drag_delta.x * drag_delta.x + drag_delta.y * drag_delta.y;

			const ImGuiStyle& style = ImGui::GetStyle();
			const float RADIUS_MIN = 80.0f;
			const float RADIUS_MAX = 160.0f;
			const float RADIUS_INTERACT_MIN = 20.0f;
			const int ITEMS_MIN = 6;
			const float item_arc_span = 2 * IM_PI / ImMax(ITEMS_MIN, numItems);

			const float inner_spacing = style.ItemInnerSpacing.x / RADIUS_MIN / 2;
			const float item_inner_ang_min = item_arc_span * (item_n - 0.5f + inner_spacing);
			const float item_inner_ang_max = item_arc_span * (item_n + 0.5f - inner_spacing);
			const float item_outer_ang_min = item_arc_span * (item_n - 0.5f + inner_spacing * (RADIUS_MIN / RADIUS_MAX));
			const float item_outer_ang_max = item_arc_span * (item_n + 0.5f - inner_spacing * (RADIUS_MIN / RADIUS_MAX));

			float drag_angle = atan2f(drag_delta.y, drag_delta.x);
			if (drag_angle < -0.5f * item_arc_span)
				drag_angle += 2.0f * IM_PI;

			bool hovered = false;
			if (drag_dist2 >= RADIUS_INTERACT_MIN * RADIUS_INTERACT_MIN) {
				if (drag_angle >= item_inner_ang_min && drag_angle < item_inner_ang_max)
					hovered = true;
			}

			WheelItem* item = _items[item_n];
			float t1 = (RADIUS_MAX - RADIUS_MIN) / 2;
			float t2 = RADIUS_MIN + t1;
			ImVec2 itemCenter = ImVec2(
				center.x + t2 * cosf(item_arc_span * item_n),
				center.y + t2 * sinf(item_arc_span * item_n)
			);
			// debug
			ImGui::GetWindowDrawList()->AddCircleFilled(itemCenter, 5, ImGui::GetColorU32(ImGuiCol_Button), 9);
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

void Wheeler::verifyWheelItems()
{
}
