#pragma once
#include "TimeInterpolator/TimeFloatInterpolator.h"
class TimeBounceInterpolator
{
private:
	TimeFloatInterpolator _interpolator;
	double _original;
	double _target;
	float _duration;
	std::function<void()> _interpolateBackToOriginal;

public:
	TimeBounceInterpolator(double originalValue);

	void InterpolateTo(double target, double duration);

	double GetValue() const;

	void ForceFinish();

	void SetValue(double value);

	void ForceValue(double value);
};
