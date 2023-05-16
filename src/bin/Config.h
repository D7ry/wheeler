#pragma once
namespace Config
{
	namespace Control
	{
		namespace Wheel
		{
			inline float cursorRadius = 60.f;
		}
	}
	namespace Styling
	{
		namespace Wheel
		{
			inline float RADIUS_SELECT_MIN = pow(60.f, 2);
			inline float RADIUS_MIN = 180.0f;
			inline float RADIUS_MAX = 360.0f;
			inline int ITEMS_MIN = 1;
			inline float ITEM_INNER_SPACING = 7.f;

			inline ImColor BackGroundColor = ImColor(0.0f, 0.0f, 0.0f, 0.3f);
			inline ImColor HoveredColor = ImColor(27, 198, 250, 125);
			inline ImColor UnhoveredColor = ImColor(0.0f, 0.0f, 0.0f, 0.5f);
			
			inline ImColor ActiveArcColor = ImColor(28, 191, 255);
			inline float ActiveArcWidth = 10.f;
		}
		
		namespace Item
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
