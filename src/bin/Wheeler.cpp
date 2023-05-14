#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Wheeler.h"
#include "Serializer.h"

#include "include/lib/PieMenu.h"

namespace TestData
{
	void LoadTestItems(std::vector<WheelItem>& r_wheelItems)
	{
		//r_wheelItems.push_back(WheelItem());
	}
}

void Wheeler::Draw()
{
	ImGui::OpenPopup("Wheeler");
	if (PieMenu::BeginPiePopup("Wheeler", 1)) {
		PieMenu::PieMenuItem("Test1");
		PieMenu::PieMenuItem("Test2");
		if (PieMenu::PieMenuItem("Test3", false))
			if (PieMenu::BeginPieMenu("Sub"))
		  {
			if (PieMenu::BeginPieMenu("Sub sub\nmenu"))
		    {
						if (PieMenu::PieMenuItem("SubSub"))
						if (PieMenu::PieMenuItem("SubSub2"))
								PieMenu::EndPieMenu();
		    }
			if (PieMenu::PieMenuItem("TestSub"))
			if (PieMenu::PieMenuItem("TestSub2"))
			PieMenu::EndPieMenu();
		  }
		PieMenu::EndPiePopup();
	}
	//for (WheelItem& item : this->_items) {
	//	item.Draw();
	//}
}

// TODO: swap to real serializer after implementing it
void Wheeler::LoadWheelItems()
{
	INFO("Loading wheel items...");
	TestData::LoadTestItems(this->_items);
	//Serializer::LoadData(_items); 
	INFO("...wheel items loaded!");
}
