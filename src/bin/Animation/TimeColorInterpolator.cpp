#include "TimeColorInterpolator.h"
#include "bin/Animation/TimeInterpolator/TimeUIntInterpolator.h"
TimeColorInterpolator::TimeColorInterpolator() :
	redInterpolator(0),
	greenInterpolator(0),
	blueInterpolator(0),
	alphaInterpolator(0) {}

TimeColorInterpolator::TimeColorInterpolator(const ImU32& targetColor) :
	redInterpolator((targetColor >> 24) & 0xFF),
	greenInterpolator((targetColor >> 16) & 0xFF),
	blueInterpolator((targetColor >> 8) & 0xFF),
	alphaInterpolator(targetColor & 0xFF) {}

void TimeColorInterpolator::InterpolateTo(const ImU32& targetColor, float duration)
{
	redInterpolator.InterpolateTo((targetColor >> 24) & 0xFF, duration);
	greenInterpolator.InterpolateTo((targetColor >> 16) & 0xFF, duration);
	blueInterpolator.InterpolateTo((targetColor >> 8) & 0xFF, duration);
	alphaInterpolator.InterpolateTo(targetColor & 0xFF, duration);
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
	uint32_t red = redInterpolator.GetValue() << 24;
	uint32_t green = greenInterpolator.GetValue() << 16;
	uint32_t blue = blueInterpolator.GetValue() << 8;
	uint32_t alpha = alphaInterpolator.GetValue();
	return red | green | blue | alpha;
}
