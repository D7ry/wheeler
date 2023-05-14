#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Wheeler.h"
#include "Serializer.h"

#include "include/lib/PieMenu.h"


#include "WheelItems/WheelItemWeapon.h"
namespace TestData
{
	void LoadTestItems(std::vector<WheelItem*>& r_wheelItems)
	{
		//r_wheelItems.push_back(WheelItem());
		r_wheelItems.emplace_back(new WheelItemWeapon());
		r_wheelItems.emplace_back(new WheelItemWeapon());
		r_wheelItems.emplace_back(new WheelItemWeapon());
	}
}

void Wheeler::Draw()
{
	ImGui::OpenPopup("Wheeler"); // open pie menu
	if (PieMenu::BeginPieMenu("Wheeler", 1)) {
		for (WheelItem* item : this->_items) {
			// it's callee's job to call PieMenuItem();
			item->Draw();
		}
		//PieMenu::PieMenuItem("Test1");
		//PieMenu::PieMenuItem("Test2");
		//if (PieMenu::BeginSubMenu("Sub")) {
		//	if (PieMenu::BeginSubMenu("Sub sub\nmenu")) {
		//		if (PieMenu::PieMenuItem("SubSub")) {
		//		}
		//		if (PieMenu::PieMenuItem("SubSub2")) {
		//		}
		//	}
		//	if (PieMenu::PieMenuItem("TestSub")) {
		//	}
		//	if (PieMenu::PieMenuItem("TestSub2")) {
		//	}
		//}
			
		PieMenu::EndPieMenu();
	}
	//for (WheelItem& item : this->_items) {
	//	item.Draw();
	//}
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

void Wheeler::verifyWheelItems()
{
}
