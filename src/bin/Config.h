#pragma once
#include "imgui.h"

static ImU32 C_SKYRIMGREY = IM_COL32(255, 255, 255, 100 );
static ImU32 C_SKYRIMWHITE = IM_COL32(255, 255, 255, 255);
static ImU32 C_BLACK = IM_COL32(0, 0, 0, 255);
static ImU32 C_SKYRIMDARKGREY_MENUBACKGROUND = IM_COL32(0, 0, 0, 125);
static ImU32 C_QUARTERTRANSPARENT = IM_COL32(255, 255, 255, (int)(255.f * .25));
static ImU32 C_HALFTRANSPARENT = IM_COL32(255, 255, 255, (int)(255.f * .5f));
static ImU32 C_TRIQUARTERTRANSPARENT = IM_COL32(255, 255, 255, (int)(255.f * .75));
static ImU32 C_VOID = IM_COL32(255, 255, 255, 0);

namespace Config
{
	#define REFERENCE_WIDTH 1920
	#define REFERENCE_HEIGHT 1080

	void ReadStyleConfig();
	void ReadControlConfig();

	void OffsetSizingToViewport();

	enum WidgetAlignment
	{
		kLeft = 0,
		kCenter = 1
	};

	namespace InputBindings
	{
		namespace GamePad
		{                                       // right thumb
			inline uint32_t nextWheel = 281;          // right trigger
			inline uint32_t prevWheel = 0;    // unmapped
			inline uint32_t toggleWheel = 280;        // left trigger
			inline uint32_t nextItem = 269;           // DPAD right
			inline uint32_t prevItem = 268;      // DPAD left
			inline uint32_t activatePrimary = 275;  // right shoulder
			inline uint32_t activateSecondary = 274;  // left shoulder
			inline uint32_t addWheel = 0;             // unmapped
			inline uint32_t addEmptyEntry = 0;        // unmapped
			inline uint32_t moveEntryForward = 0;     // unmapped
			inline uint32_t moveEntryBack = 0;        // unmapped
			inline uint32_t moveWheelForward = 0;     // unmapped
			inline uint32_t moveWheelBack = 0;        // unmapped

			inline uint32_t toggleWheelIfInInventory = 0;  // unmapped
			inline uint32_t toggleWheelIfNotInInventory = 0; // unmapped
		}
		namespace MKB
		{
			inline uint32_t nextWheel = 0x12;  // e
			inline uint32_t prevWheel = 0x10;  // q
			inline uint32_t toggleWheel = 58;  // capslock
			inline uint32_t prevItem = 264;    // mouse wheel up
			inline uint32_t nextItem = 265;    // mouse wheel down
			inline uint32_t activatePrimary = 256;  // left mouse button
			inline uint32_t activateSecondary = 257;  // right mouse button
			inline uint32_t addWheel = 49;            // N
			inline uint32_t addEmptyEntry = 50;       // M
			inline uint32_t moveEntryForward = 200;   // up arrow
			inline uint32_t moveEntryBack = 208;      // down arrow
			inline uint32_t moveWheelForward = 205;   // right arrow
			inline uint32_t moveWheelBack = 203;      // left arrow
		}
	}
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
			inline float CursorRadiusPerEntry = 10.f;
			inline bool DoubleActivateDisable = true;
			
			// if the user presses longer than this(without sending close), the wheel will close on release
			// the the user presses shorter than this, the wheel will close on a second press.
			inline float ToggleHoldThreshold = 0.25f;  

			inline bool HideGameUIInEditMode = true;
		}

	}

	namespace Animation
	{
		inline float EntryHighlightExpandTime = 0.2f;
		inline float EntryHighlightRetractTime = 0.2f;
		inline float EntryHighlightExpandScale = 0.15f;
		inline float EntryInputBumpTime = 0.1f;
		inline float EntryInputBumpScale = -0.1f;
		
		inline float ToggleVerticalFadeDistance = 0.f;
		inline float ToggleHorizontalFadeDistance = 0.f;
		inline float FadeTime = 0.08f;  // time it takes to fade in/out, set to 0 to disable.

		inline bool SnappyCursorIndicator = false;
		//inline bool CameraRotation = true;
	}

	namespace Styling
	{
		namespace Wheel
		{
			inline bool UseGeometricPrimitiveForBackgroundTexture = false;

			inline float WheelBackgroundTextureScale = 1.f;

			inline float CursorIndicatorDist = 10.f; // distance from cusor indicator to the inner circle
			inline float CusorIndicatorArcWidth = 3.f; 
			inline float CursorIndicatorArcAngle = 2 * IM_PI * 1 / 12.f;  // 1/12 of a circle
			inline float CursorIndicatorTriangleSideLength = 5.f;
			inline ImU32 CursorIndicatorColor = C_SKYRIMWHITE;
			inline bool CursorIndicatorInwardFacing = true;

			inline float WheelIndicatorOffsetX = 260.f;
			inline float WheelIndicatorOffsetY = 340.f;
			inline float WheelIndicatorSize = 10.f;
			inline float WheelIndicatorSpacing = 25.f;
			inline ImU32 WheelIndicatorActiveColor = C_SKYRIMWHITE;
			inline ImU32 WheelIndicatorInactiveColor = C_SKYRIMGREY;

			inline WidgetAlignment WheelIndicatorAlignment = WidgetAlignment::kLeft;

			inline float InnerCircleRadius = 220.0f;
			inline float OuterCircleRadius = 360.0f;
			inline float InnerSpacing = 10.f;

			inline ImU32 HoveredColorBegin = C_QUARTERTRANSPARENT;
			inline ImU32 HoveredColorEnd = C_HALFTRANSPARENT;

			inline ImU32 UnhoveredColorBegin = C_SKYRIMDARKGREY_MENUBACKGROUND;
			inline ImU32 UnhoveredColorEnd = C_SKYRIMDARKGREY_MENUBACKGROUND;

			inline ImU32 ActiveArcColorBegin = C_SKYRIMWHITE;
			inline ImU32 ActiveArcColorEnd = C_SKYRIMWHITE;
			
			inline ImU32 InActiveArcColorBegin = C_SKYRIMGREY;
			inline ImU32 InActiveArcColorEnd = C_SKYRIMGREY;

			inline float ActiveArcWidth = 7.f;
			
			inline bool BlurOnOpen = true;
			inline float SlowTimeScale = .1f;

			
			// offset of wheel center, to which everything else is relative to
			inline float CenterOffsetX = 450.f;
			inline float CenterOffsetY = 0.f;

			inline ImU32 TextColor = C_SKYRIMWHITE;
			inline ImU32 TextShadowColor = C_BLACK;
		}

		namespace Entry
		{
			namespace Highlight
			{
				namespace Text
				{
					inline float OffsetX = 0;
					inline float OffsetY = -130;
					inline float Size = 27;
				}
			}

		}
		
		namespace Item
		{
			namespace Highlight
			{
				namespace Texture
				{
					inline float OffsetX = 0;
					inline float OffsetY = -50;
					inline float Scale = .2f;
				}

				namespace Text
				{
					inline float OffsetX = 0;
					inline float OffsetY = 20;
					inline float Size = 35;
				}

				namespace Desc
				{
					inline float OffsetX = 0;
					inline float OffsetY = 50;
					inline float Size = 30;
					inline float LineLength = 500.f;
					inline float LineSpacing = 5.f;
				}

				namespace StatIcon
				{
					inline float OffsetX = 0;
					inline float OffsetY = 0;
					inline float Scale = .2f;
				}

				namespace StatText
				{
					inline float OffsetX = 0;
					inline float OffsetY = 0;
					inline float Size = 35;
				}
			}
			namespace Slot
			{
				namespace Texture
				{
					inline float OffsetX = 0;
					inline float OffsetY = -25;
					inline float Scale = .1f;
				}

				namespace Text
				{
					inline float OffsetX = 0;
					inline float OffsetY = 10;
					inline float Size = 30;
				}

				namespace BackgroundTexture
				{
					inline float Scale = .1f;
				}

			}
			
		}

		
	}

}
