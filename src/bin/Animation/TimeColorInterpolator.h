#pragma once
#include "imgui.h"
#include "bin/Animation/TimeInterpolator/TimeUIntInterpolator.h"
class TimeColorInterpolator
{
private:
	TimeUintInterpolator redInterpolator;
	TimeUintInterpolator greenInterpolator;
	TimeUintInterpolator blueInterpolator;
	TimeUintInterpolator alphaInterpolator;

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
