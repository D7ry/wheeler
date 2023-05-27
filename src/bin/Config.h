#pragma once
#include "Wheeler.h"
#include "imgui.h"

static ImU32 C_SKYRIMGREY = IM_COL32(255, 255, 255, 100 );
static ImU32 C_SKYRIMWHITE = IM_COL32(255, 255, 255, 255);
static ImU32 C_SKYRIMDARKGREY_MENUBACKGROUND = IM_COL32(0, 0, 0, 125);
static ImU32 C_QUARTERTRANSPARENT = IM_COL32(255, 255, 255, (int)(255.f * .25));
static ImU32 C_HALFTRANSPARENT = IM_COL32(255, 255, 255, (int)(255.f * .5f));
static ImU32 C_TRIQUARTERTRANSPARENT = IM_COL32(255, 255, 255, (int)(255.f * .75));
static ImU32 C_VOID = IM_COL32(255, 255, 255, 0);

namespace Config
{
	namespace Sound
	{
		static inline const char* SD_WHEELSWITCH = "UIFavorite";
		static inline const char* SD_ENTRYSWITCH = "UIMenuFocus";
		static inline const char* SD_WHEELERTOGGLE = "UIInventoryOpenSD";
		static inline const char* SD_ITEMSWITCH = "UIMenuPrevNextSD";

	}
	namespace Control
	{
		namespace Wheel
		{
			// radius that bounds the mouse cursor. Increases with each entry in wheel to make sure MKB users don't rotate the cursor too fast.
			inline float CursorRadiusPerEntry = 15.f;
			inline bool DoubleActivateDisable = true;
		}

	}
	namespace Sound
	{
		inline bool ItemSwitchSound = true;
		inline bool EntrySwitchSound = true;
		inline bool WheelSwitchSound = true;
		inline bool WheelToggleSound = true;

	}
	namespace Styling
	{
		namespace Wheel
		{
			inline float CursorIndicatorDist = 10.f; // distance from cusor indicator to the inner circle
			inline float CusorIndicatorArcWidth = 3.f; 
			inline float CursorIndicatorArcAngle = 2 * IM_PI * 1 / 12.f;  // 1/12 of a circle
			inline float CursorIndicatorTriangleSideLength = 5.f;
			inline ImU32 CursorIndicatorColor = C_SKYRIMWHITE;

			inline float WheelIndicatorOffsetX = 260.f;
			inline float WheelIndicatorOffsetY = 340.f;
			inline float WheelIndicatorSize = 10.f;
			inline float WheelIndicatorSpacing = 25.f;
			inline ImU32 WheelIndicatorActiveColor = C_SKYRIMWHITE;
			inline ImU32 WheelIndicatorInactiveColor = C_SKYRIMGREY;

			inline float RADIUS_SELECT_MIN = pow(60.f, 2);
			inline float InnerCircleRadius = 220.0f;
			inline float OuterCircleRadius = 360.0f;
			inline int ITEMS_MIN = 1;
			inline float InnerSpacing = 10.f;

			inline ImU32 BackGroundColor = ImColor(0.0f, 0.0f, 0.0f, 0.5f);
			inline ImU32 HoveredColorBegin = C_QUARTERTRANSPARENT;
			inline ImU32 HoveredColorEnd = C_HALFTRANSPARENT;

			inline ImU32 UnhoveredColorBegin = C_SKYRIMDARKGREY_MENUBACKGROUND;
			inline ImU32 UnhoveredColorEnd = C_SKYRIMDARKGREY_MENUBACKGROUND;

			inline ImU32 ActiveArcColorBegin = C_SKYRIMWHITE;
			inline ImU32 ActiveArcColorEnd = C_SKYRIMWHITE;
			
			inline ImU32 InActiveArcColorBegin = C_SKYRIMGREY;
			inline ImU32 InActiveArcColorEnd = C_SKYRIMGREY;

			inline float ActiveArcWidth = 7.f;

			inline uint32_t NumWheels = 3;  // # of wheels
			
			inline bool BlurOnOpen = true;
			inline float SlowTimeScale = .1f;

			inline float FadeTime = 0.08f;  // time it takes to fade in/out, set to 0 to disable.
			
			// offset of wheel center, to which everything else is relative to
			inline float CenterOffsetX = 450.f;
			inline float CenterOffsetY = 0.f;
		}

		namespace Entry
		{
			namespace Highlight
			{
				namespace Text
				{
					const inline float OffsetX = 0;
					const inline float OffsetY = -130;
					const inline float Size = 27;
				}
			}
		}
		
		namespace Item
		{
			namespace Highlight
			{
				namespace Texture
				{
					const inline float OffsetX = 0;
					const inline float OffsetY = -50;
					const inline float Scale = .2f;
				}

				namespace Text
				{
					const inline float OffsetX = 0;
					const inline float OffsetY = 20;
					const inline float Size = 35;
				}

				namespace Desc
				{
					const inline float OffsetX = 0;
					const inline float OffsetY = 30;
					const inline float Size = 15;
				}
			}
			namespace Slot
			{
				namespace Texture
				{
					const inline float OffsetX = 0;
					const inline float OffsetY = -25;
					const inline float Scale = .1f;
				}

				namespace Text
				{
					const inline float OffsetX = 0;
					const inline float OffsetY = 10;
					const inline float Size = 30;
				}

				namespace Desc
				{
					const inline float OffsetX = 0;
					const inline float OFfsetY = 30;
					const inline float Size = 20;
				}
			}
			
		}

		
	}

}
