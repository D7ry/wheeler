#include "TimeTrapezoidInterpolator.h"

TimeTrapezoidInterpolator::TimeTrapezoidInterpolator(float initialPoint1, float initialPoint2, float initialPoint3, float initialPoint4) :
	point1Interpolator(initialPoint1),
	point2Interpolator(initialPoint2),
	point3Interpolator(initialPoint3),
	point4Interpolator(initialPoint4) {}

void TimeTrapezoidInterpolator::InterpolateTo(float targetPoint1, float targetPoint2, float targetPoint3, float targetPoint4, float duration)
{
	point1Interpolator.InterpolateTo(targetPoint1, duration);
	point2Interpolator.InterpolateTo(targetPoint2, duration);
	point3Interpolator.InterpolateTo(targetPoint3, duration);
	point4Interpolator.InterpolateTo(targetPoint4, duration);
}

float TimeTrapezoidInterpolator::GetPoint1() const
{
	return point1Interpolator.GetValue();
}

float TimeTrapezoidInterpolator::GetPoint2() const
{
	return point2Interpolator.GetValue();
}

float TimeTrapezoidInterpolator::GetPoint3() const
{
	return point3Interpolator.GetValue();
}

float TimeTrapezoidInterpolator::GetPoint4() const
{
	return point4Interpolator.GetValue();
}
