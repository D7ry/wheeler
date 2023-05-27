#pragma once
#include "imgui.h"
#include "bin/Animation/TimeInterpolator/TimeFloatInterpolator.h"

/**
 * Interpolates ImU32 color value over time through 4 instances of TimeUintInterpolator.
 */
class TimeColorInterpolator
{
private:
	TimeFloatInterpolator redInterpolator;
	TimeFloatInterpolator greenInterpolator;
	TimeFloatInterpolator blueInterpolator;
	TimeFloatInterpolator alphaInterpolator;

public:
	TimeColorInterpolator();
	TimeColorInterpolator(const ImU32& targetColor);

	void InterpolateTo(const ImU32& targetColor, float duration);

	uint32_t GetRed();
	uint32_t GetGreen();
	uint32_t GetBlue();
	uint32_t GetAlpha();
	uint32_t GetColor();
};
