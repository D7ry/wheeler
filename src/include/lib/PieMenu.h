#pragma once
#include <imgui_internal.h>
class PieMenuItem
{
public:
	virtual void Draw();
	virtual void Activate(uint32_t keyID);
};
namespace PieMenu
{
	// Return >= 0 on mouse release
	// Optional int* p_selected display and update a currently selected item
	int PiePopupSelectMenu(const ImVec2& center, const char* popup_id, const char** items, int items_count, int* p_selected, bool shouldClose);

	void DrawPieMenu(const ImVec2& center, const char* popup_id, std::vector<PieMenuItem*>& items, bool shouldClose);
}
