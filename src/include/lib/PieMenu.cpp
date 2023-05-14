#include "PieMenu.h"

// Namespace for the pie menu functionality
namespace PieMenu
{
	// Structure defining the properties of a pie menu
	struct PieMenuContext
	{
		// Constants
		static const int c_iMaxPieMenuStack = 8;       // Maximum number of nested pie menus
		static const int c_iMaxPieItemCount = 12;      // Maximum number of items in a pie menu
		static const int c_iRadiusEmpty = 30;          // Radius for an empty pie menu
		static const int c_iRadiusMin = 30;            // Minimum radius for a pie menu
		static const int c_iMinItemCount = 3;          // Minimum number of items in a pie menu
		static const int c_iMinItemCountPerLevel = 3;  // Minimum number of items per level in a nested pie menu

		// Structure defining a single pie menu
		struct PieMenu
		{
			// Data members
			int m_iCurrentIndex;                              // Index of the current item being added
			float m_fMaxItemSqrDiameter;                      // Maximum squared diameter among all items
			float m_fLastMaxItemSqrDiameter;                  // Maximum squared diameter of the previous item
			int m_iHoveredItem;                               // Index of the currently hovered item
			int m_iLastHoveredItem;                           // Index of the previously hovered item
			int m_iClickedItem;                               // Index of the clicked item
			bool m_oItemIsSubMenu[c_iMaxPieItemCount];        // Flags indicating whether each item is a sub-menu
			ImVector<char> m_oItemNames[c_iMaxPieItemCount];  // Names of the items
			ImVec2 m_oItemSizes[c_iMaxPieItemCount];          // Sizes of the items
		};

		// Constructor
		PieMenuContext()
		{
			m_iCurrentIndex = -1;
			m_iLastFrame = 0;
		}

		// Data members
		PieMenu m_oPieMenuStack[c_iMaxPieMenuStack];  // Stack of pie menus
		int m_iCurrentIndex;                          // Index of the current pie menu in the stack
		int m_iMaxIndex;                              // Maximum index reached in the stack
		int m_iLastFrame;                             // Last frame number when the pie menu was active
		ImVec2 m_oCenter;                             // Center position of the pie menu
		int m_iMouseButton;                           // Mouse button used to interact with the pie menu
		bool m_bClose;                                // Flag indicating whether the pie menu should be closed
	};

	// Static instance of the pie menu context
	static PieMenuContext s_oPieMenuContext;

	// Function to check if a popup menu is open
	bool IsPopupOpen(const char* pName)
	{
		ImGuiID iID = ImGui::GetID(pName);
		ImGuiContext& g = *GImGui;
		return g.OpenPopupStack.Size > g.BeginPopupStack.Size && g.OpenPopupStack[g.BeginPopupStack.Size].PopupId == iID;
	}

	// Begin a pie menu
	void BeginPieMenuEx()
	{
		// Check the maximum number of pie menus allowed
		IM_ASSERT(s_oPieMenuContext.m_iCurrentIndex < PieMenuContext::c_iMaxPieMenuStack);

		// Increment the current and maximum index
		++s_oPieMenuContext.m_iCurrentIndex;
		++s_oPieMenuContext.m_iMaxIndex;

		// Retrieve the currentPie menu from the context stack
		PieMenuContext::PieMenu& oPieMenu = s_oPieMenuContext.m_oPieMenuStack[s_oPieMenuContext.m_iCurrentIndex];
		// Initialize the pie menu properties
		oPieMenu.m_iCurrentIndex = 0;
		oPieMenu.m_fMaxItemSqrDiameter = 0.f;

		// Reset the hovered and clicked items
		if (!ImGui::IsMouseReleased(s_oPieMenuContext.m_iMouseButton))
			oPieMenu.m_iHoveredItem = -1;
		if (s_oPieMenuContext.m_iCurrentIndex > 0)
			oPieMenu.m_fMaxItemSqrDiameter = s_oPieMenuContext.m_oPieMenuStack[s_oPieMenuContext.m_iCurrentIndex - 1].m_fMaxItemSqrDiameter;
	}

	// End a pie menu
	void EndPieMenuEx()
	{
		// Check that there is a pie menu active
		IM_ASSERT(s_oPieMenuContext.m_iCurrentIndex >= 0);

		// Get a reference to the current pie menu
		PieMenuContext::PieMenu& oPieMenu = s_oPieMenuContext.m_oPieMenuStack[s_oPieMenuContext.m_iCurrentIndex];

		// Decrement the current index to pop the pie menu from the stack
		--s_oPieMenuContext.m_iCurrentIndex;
	}

	// Begin a pie menu with a specified name and mouse button
	bool BeginPieMenu(const char* pName, int iMouseButton)
	{
		// Check if the popup menu with the given name is open
		if (IsPopupOpen(pName)) {
			// Push style and settings for the pie menu appearance
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

			// Set the mouse button and close flag from the context
			s_oPieMenuContext.m_iMouseButton = iMouseButton;
			s_oPieMenuContext.m_bClose = false;

			// Set the position of the pie menu window outside the screen to avoid flickering
			ImGui::SetNextWindowPos(ImVec2(-100.f, -100.f), ImGuiCond_Appearing);

			// Begin the popup window with the given name and flags
			bool bOpened = ImGui::BeginPopup(pName, ImGuiWindowFlags_NoBackground);

			// If the popup window is open
			if (bOpened) {
				int iCurrentFrame = ImGui::GetFrameCount();

				// Center the pie menu on the screen on the first frame it appears
				if (s_oPieMenuContext.m_iLastFrame < (iCurrentFrame - 1)) {
					auto display = ImGui::GetIO().DisplaySize;
					ImVec2 center = ImVec2(display.x * 0.5f, display.y * 0.5f);
					s_oPieMenuContext.m_oCenter = center;
					SetCursorPos(center.x, center.y);
				}

				// Update the last frame number
				s_oPieMenuContext.m_iLastFrame = iCurrentFrame;

				// Reset the maximum index and begin the pie menu
				s_oPieMenuContext.m_iMaxIndex = -1;
				BeginPieMenuEx();
				return true;
			} else {
				// If the popup window is not open, end the window and pop the style settings
				ImGui::End();
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(2);
			}
		}
		return false;
	}

	// End a pie menu
	void EndPieMenu()
	{
		// End the current pie menu
		EndPieMenuEx();

		// Retrieve the ImGui style and texture UV for a white pixel
		ImGuiStyle& oStyle = ImGui::GetStyle();
		const ImVec2& vTexUvWhitePixel = ImGui::GetDrawListSharedData()->TexUvWhitePixel;

		// Retrieve the draw list for the current ImGui window
		ImDrawList* pDrawList = ImGui::GetWindowDrawList();
		pDrawList->PushClipRectFullScreen();

		// Get the mouse position and calculate the drag delta
		const ImVec2 oMousePos = ImGui::GetIO().MousePos;
		const ImVec2 oDragDelta = ImVec2(oMousePos.x - s_oPieMenuContext.m_oCenter.x, oMousePos.y - s_oPieMenuContext.m_oCenter.y);
		const float fDragDistSqr = oDragDelta.x * oDragDelta.x + oDragDelta.y * oDragDelta.y;

		float fCurrentRadius = (float)PieMenuContext::c_iRadiusEmpty;

		ImRect oArea = ImRect(s_oPieMenuContext.m_oCenter, s_oPieMenuContext.m_oCenter);

		bool bItemHovered = false;

		const float c_fDefaultRotate = -IM_PI / 2.f;
		float fLastRotate = c_fDefaultRotate;

		// Iterate through the pie menus in the context stack
		for (int iIndex = 0; iIndex <= s_oPieMenuContext.m_iMaxIndex; ++iIndex) {
			// Get the current pie menu from the stack
			PieMenuContext::PieMenu& oPieMenu = s_oPieMenuContext.m_oPieMenuStack[iIndex];

			// Calculate the height of the pie menu based on the maximum item diameter
			float fMenuHeight = sqrt(oPieMenu.m_fMaxItemSqrDiameter);

			const float fMinRadius = fCurrentRadius;
			const float fMaxRadius = fMinRadius + (fMenuHeight * oPieMenu.m_iCurrentIndex) / (2.f);

			const float item_arc_span = 2 * IM_PI / ImMax(PieMenuContext::c_iMinItemCount + PieMenuContext::c_iMinItemCountPerLevel * iIndex, oPieMenu.m_iCurrentIndex);
			float drag_angle = atan2f(oDragDelta.y, oDragDelta.x);

			float fRotate = fLastRotate - item_arc_span * (oPieMenu.m_iCurrentIndex - 1.f) / 2.f;
			int item_hovered = -1;

			// Iterate through the items in the pie menu
			for (int item_n = 0; item_n < oPieMenu.m_iCurrentIndex; item_n++) {
				// Retrieve the label and size of the item
				const char* item_label = oPieMenu.m_oItemNames[item_n].Data;
				const float inner_spacing = oStyle.ItemInnerSpacing.x / fMinRadius / 2;
				const float fMinInnerSpacing = oStyle.ItemInnerSpacing.x / (fMinRadius * 2.f);
				const float fMaxInnerSpacing = oStyle.ItemInnerSpacing.x / (fMaxRadius * 2.f);
				const float item_inner_ang_min = item_arc_span * (item_n - 0.5f + fMinInnerSpacing) + fRotate;
				const float item_inner_ang_max = item_arc_span * (item_n + 0.5f - fMinInnerSpacing) + fRotate;
				const float item_outer_ang_min = item_arc_span * (item_n - 0.5f + fMaxInnerSpacing) + fRotate;
				const float item_outer_ang_max = item_arc_span * (item_n + 0.5f - fMaxInnerSpacing) + fRotate;
				bool hovered = false;

				// Check if the mouse drag position falls within the item's inner angle range
				if (fDragDistSqr >= fMinRadius * fMinRadius && fDragDistSqr < fMaxRadius * fMaxRadius) {
					while ((drag_angle - item_inner_ang_min) < 0.f)
						drag_angle += 2.f * IM_PI;
					while ((drag_angle - item_inner_ang_min) > 2.f * IM_PI)
						drag_angle -= 2.f * IM_PI;

					if (drag_angle >= item_inner_ang_min && drag_angle < item_inner_ang_max) {
						hovered = true;
						bItemHovered = !oPieMenu.m_oItemIsSubMenu[item_n];
					}
				}

				int arc_segments = (int)(32 * item_arc_span / (2 * IM_PI)) + 1;

				ImU32 iColor = hovered ? ImColor(100, 100, 150) : ImColor(70, 70, 70);
				iColor = ImGui::GetColorU32(hovered ? ImGuiCol_HeaderHovered : ImGuiCol_FrameBg);
				iColor = ImGui::GetColorU32(hovered ? ImGuiCol_Button : ImGuiCol_ButtonHovered);

				const float fAngleStepInner = (item_inner_ang_max - item_inner_ang_min) / arc_segments;
				const float fAngleStepOuter = (item_outer_ang_max - item_outer_ang_min) / arc_segments;
				pDrawList->PrimReserve(arc_segments * 6, (arc_segments + 1) * 2);
				for (int iSeg = 0; iSeg <= arc_segments; ++iSeg) {
					float fCosInner = cosf(item_inner_ang_min + fAngleStepInner * iSeg);
					float fSinInner = sinf(item_inner_ang_min + fAngleStepInner * iSeg);
					float fCosOuter = cosf(item_outer_ang_min + fAngleStepOuter * iSeg);
					float fSinOuter = sinf(item_outer_ang_min + fAngleStepOuter * iSeg);

					if (iSeg < arc_segments) {
						pDrawList->PrimWriteIdx(pDrawList->_VtxCurrentIdx + 0);
						pDrawList->PrimWriteIdx(pDrawList->_VtxCurrentIdx + 2);
						pDrawList->PrimWriteIdx(pDrawList->_VtxCurrentIdx + 1);
						pDrawList->PrimWriteIdx(pDrawList->_VtxCurrentIdx + 3);
						pDrawList->PrimWriteIdx(pDrawList->_VtxCurrentIdx + 2);
						pDrawList->PrimWriteIdx(pDrawList->_VtxCurrentIdx + 1);
					}
					pDrawList->PrimWriteVtx(ImVec2(s_oPieMenuContext.m_oCenter.x + fCosInner * (fMinRadius + oStyle.ItemInnerSpacing.x), s_oPieMenuContext.m_oCenter.y + fSinInner * (fMinRadius + oStyle.ItemInnerSpacing.x)), vTexUvWhitePixel, iColor);
					pDrawList->PrimWriteVtx(ImVec2(s_oPieMenuContext.m_oCenter.x + fCosOuter * (fMaxRadius - oStyle.ItemInnerSpacing.x), s_oPieMenuContext.m_oCenter.y + fSinOuter * (fMaxRadius - oStyle.ItemInnerSpacing.x)), vTexUvWhitePixel, iColor);
				}
				float fRadCenter = (item_arc_span * item_n) + fRotate;
				ImVec2 oOuterCenter = ImVec2(s_oPieMenuContext.m_oCenter.x + cosf(fRadCenter) * fMaxRadius, s_oPieMenuContext.m_oCenter.y + sinf(fRadCenter) * fMaxRadius);
				oArea.Add(oOuterCenter);

				if (oPieMenu.m_oItemIsSubMenu[item_n]) {
					ImVec2 oTrianglePos[3];

					float fRadLeft = fRadCenter - 5.f / fMaxRadius;
					float fRadRight = fRadCenter + 5.f / fMaxRadius;

					oTrianglePos[0].x = s_oPieMenuContext.m_oCenter.x + cosf(fRadCenter) * (fMaxRadius - 5.f);
					oTrianglePos[0].y = s_oPieMenuContext.m_oCenter.y + sinf(fRadCenter) * (fMaxRadius - 5.f);
					oTrianglePos[1].x = s_oPieMenuContext.m_oCenter.x + cosf(fRadLeft) * (fMaxRadius - 10.f);
					oTrianglePos[1].y = s_oPieMenuContext.m_oCenter.y + sinf(fRadLeft) * (fMaxRadius - 10.f);
					oTrianglePos[2].x = s_oPieMenuContext.m_oCenter.x + cosf(fRadRight) * (fMaxRadius - 10.f);
					oTrianglePos[2].y = s_oPieMenuContext.m_oCenter.y + sinf(fRadRight) * (fMaxRadius - 10.f);

					pDrawList->AddTriangleFilled(oTrianglePos[0], oTrianglePos[1], oTrianglePos[2], ImColor(255, 255, 255));
				}

				ImVec2 text_size = oPieMenu.m_oItemSizes[item_n];
				ImVec2 text_pos = ImVec2(
					s_oPieMenuContext.m_oCenter.x + cosf((item_inner_ang_min + item_inner_ang_max) * 0.5f) * (fMinRadius + fMaxRadius) * 0.5f - text_size.x * 0.5f,
					s_oPieMenuContext.m_oCenter.y + sinf((item_inner_ang_min + item_inner_ang_max) * 0.5f) * (fMinRadius + fMaxRadius) * 0.5f - text_size.y * 0.5f);
				pDrawList->AddText(text_pos, ImColor(255, 255, 255), item_label);

				if (hovered)
					item_hovered = item_n;
			}

			fCurrentRadius = fMaxRadius;

			oPieMenu.m_fLastMaxItemSqrDiameter = oPieMenu.m_fMaxItemSqrDiameter;

			oPieMenu.m_iHoveredItem = item_hovered;

			if (fDragDistSqr >= fMaxRadius * fMaxRadius)
				item_hovered = oPieMenu.m_iLastHoveredItem;

			oPieMenu.m_iLastHoveredItem = item_hovered;

			fLastRotate = item_arc_span * oPieMenu.m_iLastHoveredItem + fRotate;
			// If the hovered item is a submenu, continue to the next level
			if (item_hovered == -1 || !oPieMenu.m_oItemIsSubMenu[item_hovered])
				break;
		}

		pDrawList->PopClipRect();

		// Adjust the center position of the pie menu if it goes beyond the screen boundaries
		if (oArea.Min.x < 0.f) {
			s_oPieMenuContext.m_oCenter.x = (s_oPieMenuContext.m_oCenter.x - oArea.Min.x);
		}
		if (oArea.Min.y < 0.f) {
			s_oPieMenuContext.m_oCenter.y = (s_oPieMenuContext.m_oCenter.y - oArea.Min.y);
		}

		ImVec2 oDisplaySize = ImGui::GetIO().DisplaySize;
		if (oArea.Max.x > oDisplaySize.x) {
			s_oPieMenuContext.m_oCenter.x = (s_oPieMenuContext.m_oCenter.x - oArea.Max.x) + oDisplaySize.x;
		}
		if (oArea.Max.y > oDisplaySize.y) {
			s_oPieMenuContext.m_oCenter.y = (s_oPieMenuContext.m_oCenter.y - oArea.Max.y) + oDisplaySize.y;
		}

		// Close the pie menu if the close flag is set or if the mouse button is released outside the menu
		if (s_oPieMenuContext.m_bClose ||
			(!bItemHovered && ImGui::IsMouseReleased(s_oPieMenuContext.m_iMouseButton))) {
			ImGui::CloseCurrentPopup();
		}

		// End the popup window
		ImGui::EndPopup();
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);
	}

	// Begin a submenu within the current pie menu
	bool BeginSubMenu(const char* pName, bool bEnabled)
	{
		// Ensure that there is a valid current index within the pie menu context
		IM_ASSERT(s_oPieMenuContext.m_iCurrentIndex >= 0 && s_oPieMenuContext.m_iCurrentIndex < PieMenuContext::c_iMaxPieItemCount);

		// Get the current pie menu from the context stack
		PieMenuContext::PieMenu& oPieMenu = s_oPieMenuContext.m_oPieMenuStack[s_oPieMenuContext.m_iCurrentIndex];

		// Calculate and store the size of the submenu item
		ImVec2 oTextSize = ImGui::CalcTextSize(pName, NULL, true);
		oPieMenu.m_oItemSizes[oPieMenu.m_iCurrentIndex] = oTextSize;

		// Calculate the squared diameter of the submenu item
		float fSqrDiameter = oTextSize.x * oTextSize.x + oTextSize.y * oTextSize.y;

		// Update the maximum item diameter if necessary
		if (fSqrDiameter > oPieMenu.m_fMaxItemSqrDiameter) {
			oPieMenu.m_fMaxItemSqrDiameter = fSqrDiameter;
		}

		// Set the submenu flag for the current item
		oPieMenu.m_oItemIsSubMenu[oPieMenu.m_iCurrentIndex] = true;
		// Copy the name of the submenu item
		int iLen = strlen(pName);
		ImVector<char>& oName = oPieMenu.m_oItemNames[oPieMenu.m_iCurrentIndex];
		oName.resize(iLen + 1);
		oName[iLen] = '\0';
		memcpy(oName.Data, pName, iLen);

		// If the last hovered item is the current item, begin the submenu by incrementing the current index and calling BeginPieMenuEx()
		if (oPieMenu.m_iLastHoveredItem == oPieMenu.m_iCurrentIndex) {
			++oPieMenu.m_iCurrentIndex;
			BeginPieMenuEx();
			return true;
		}

		// Increment the current index
		++oPieMenu.m_iCurrentIndex;

		return false;
	}

	// End a submenu within the current pie menu
	void EndSubMenu()
	{
		// Ensure that there is a valid current index within the pie menu context
		IM_ASSERT(s_oPieMenuContext.m_iCurrentIndex >= 0 && s_oPieMenuContext.m_iCurrentIndex < PieMenuContext::c_iMaxPieItemCount);

		// Decrement the current index to end the submenu
		--s_oPieMenuContext.m_iCurrentIndex;
	}

	// Render a pie menu item within the current pie menu
	bool PieMenuItem(const char* pName, bool bEnabled)
	{
		// Ensure that there is a valid current index within the pie menu context
		IM_ASSERT(s_oPieMenuContext.m_iCurrentIndex >= 0 && s_oPieMenuContext.m_iCurrentIndex < PieMenuContext::c_iMaxPieItemCount);

		// Get the current pie menu from the context stack
		PieMenuContext::PieMenu& oPieMenu = s_oPieMenuContext.m_oPieMenuStack[s_oPieMenuContext.m_iCurrentIndex];

		// Calculate and store the size of the menu item
		ImVec2 oTextSize = ImGui::CalcTextSize(pName, NULL, true);
		oPieMenu.m_oItemSizes[oPieMenu.m_iCurrentIndex] = oTextSize;

		// Calculate the squared diameter of the menu item
		float fSqrDiameter = oTextSize.x * oTextSize.x + oTextSize.y * oTextSize.y;

		// Update the maximum item diameter if necessary
		if (fSqrDiameter > oPieMenu.m_fMaxItemSqrDiameter) {
			oPieMenu.m_fMaxItemSqrDiameter = fSqrDiameter;
		}

		// Set the submenu flag for the current item
		oPieMenu.m_oItemIsSubMenu[oPieMenu.m_iCurrentIndex] = false;

		// Copy the name of the menu item
		int iLen = strlen(pName);
		ImVector<char>& oName = oPieMenu.m_oItemNames[oPieMenu.m_iCurrentIndex];
		oName.resize(iLen + 1);
		oName[iLen] = '\0';
		memcpy(oName.Data, pName, iLen);

		// Determine if the menu item is currently active (hovered)
		bool bActive = oPieMenu.m_iCurrentIndex == oPieMenu.m_iHoveredItem;

		// Increment the current index
		++oPieMenu.m_iCurrentIndex;

		// Set the close flag if the menu item is active
		if (bActive)
			s_oPieMenuContext.m_bClose = true;
		// Return the active state of the menu item
		return bActive;
	}
}
