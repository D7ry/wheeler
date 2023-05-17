#pragma once
#include "Wheeler.h"
#include "imgui.h"
namespace Config
{
	
	namespace Control
	{
		namespace Wheel
		{
			inline float cursorRadius = 60.f;
			inline bool DoubleActivateDisable = true;
		}

	}
	namespace Styling
	{
		namespace Wheel
		{
			inline float RADIUS_SELECT_MIN = pow(60.f, 2);
			inline float RADIUS_MIN = 220.0f;
			inline float RADIUS_MAX = 360.0f;
			inline int ITEMS_MIN = 1;
			inline float ITEM_INNER_SPACING = 5.f;

			inline ImColor BackGroundColor = ImColor(0.0f, 0.0f, 0.0f, 0.5f);
			inline ImColor HoveredColorBegin = ImColor(105, 177, 207, 25);
			inline ImColor HoveredColorEnd = ImColor(105, 177, 207, 150);

			inline ImColor UnhoveredColorBegin = ImColor(0.0f, 0.0f, 0.0f, 0.3f);
			inline ImColor UnhoveredColorEnd = ImColor(0.0f, 0.0f, 0.0f, 0.3f);

			inline ImColor ActiveArcColorBegin = ImColor(105,177,207);
			inline ImColor ActiveArcColorEnd = ImColor(105, 177, 207);
			
			inline ImColor InActiveArcColorBegin = ImColor(77, 79, 80, 200);
			inline ImColor InActiveArcColorEnd = ImColor(77, 79, 80, 200);

			inline float ActiveArcWidth = 10.f;

			inline uint32_t NumWheels = 3;  // # of wheels
		}

		namespace Entry
		{
			namespace Highlight
			{
				namespace Text
				{
					const inline float OffsetX = 0;
					const inline float OffsetY = -130;
					const inline float Size = 20;
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
					const inline float Size = 30;
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
					const inline float Size = 20;
				}

				namespace Desc
				{
					const inline float OffsetX = 0;
					const inline float OFfsetY = 30;
					const inline float Size = 15;
				}
			}
			
		}

		
	}

}
