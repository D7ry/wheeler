#pragma once
#include "TimeInterpolator/TimeFloatInterpolator.h"
class TimeTrapezoidInterpolator
{
private:
	TimeFloatInterpolator point1Interpolator;
	TimeFloatInterpolator point2Interpolator;
	TimeFloatInterpolator point3Interpolator;
	TimeFloatInterpolator point4Interpolator;

public:
	TimeTrapezoidInterpolator() :
		point1Interpolator(0.0f),
		point2Interpolator(0.0f),
		point3Interpolator(0.0f),
		point4Interpolator(0.0f) {}

	TimeTrapezoidInterpolator(float initialPoint1, float initialPoint2, float initialPoint3, float initialPoint4);

	void InterpolateTo(float targetPoint1, float targetPoint2, float targetPoint3, float targetPoint4, float duration);

	float GetPoint1() const;

	float GetPoint2() const;

	float GetPoint3() const;

	float GetPoint4() const;
};
