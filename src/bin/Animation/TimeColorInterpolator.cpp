#include "TimeColorInterpolator.h"
#include "bin/Animation/TimeInterpolator/TimeUIntInterpolator.h"
TimeColorInterpolator::TimeColorInterpolator() :
	redInterpolator(0),
	greenInterpolator(0),
	blueInterpolator(0),
	alphaInterpolator(0) {}

TimeColorInterpolator::TimeColorInterpolator(const ImU32& targetColor) :
	redInterpolator((targetColor >> IM_COL32_R_SHIFT) & 0xFF),
	greenInterpolator((targetColor >> IM_COL32_G_SHIFT) & 0xFF),
	blueInterpolator((targetColor >> IM_COL32_B_SHIFT) & 0xFF),
	alphaInterpolator((targetColor >> IM_COL32_A_SHIFT) & 0xFF) {}

void TimeColorInterpolator::InterpolateTo(const ImU32& targetColor, float duration)
{
	// assuming the targetColor is in ARGB format
	uint32_t a = (targetColor >> IM_COL32_A_SHIFT) & 0xFF;
	uint32_t b = (targetColor >> IM_COL32_B_SHIFT) & 0xFF;
	uint32_t g = (targetColor >> IM_COL32_G_SHIFT) & 0xFF;
	uint32_t r = (targetColor >> IM_COL32_R_SHIFT) & 0xFF;

	redInterpolator.InterpolateTo(r, duration);
	greenInterpolator.InterpolateTo(g, duration);
	blueInterpolator.InterpolateTo(b, duration);
	alphaInterpolator.InterpolateTo(a, duration);
}

uint32_t TimeColorInterpolator::GetRed()
{
	return redInterpolator.GetValue();
}

uint32_t TimeColorInterpolator::GetGreen()
{
	return greenInterpolator.GetValue();
}

uint32_t TimeColorInterpolator::GetBlue()
{
	return blueInterpolator.GetValue();
}

uint32_t TimeColorInterpolator::GetAlpha()
{
	return alphaInterpolator.GetValue();
}

uint32_t TimeColorInterpolator::GetColor()
{
	uint32_t a = GetAlpha();
	uint32_t r = GetRed();
	uint32_t g = GetGreen();
	uint32_t b = GetBlue();
	return IM_COL32(r, g, b, a);
}
