#include "TimeBounceInterpolator.h"

TimeBounceInterpolator::TimeBounceInterpolator(double originalValue) :
	_original(originalValue)
{
	// Register the callback function to reverse the interpolating direction
	_interpolateBackToOriginal = [this]() {
		_interpolator.InterpolateTo(_original, _duration);
	};
	std::function<void()> funcReset = [this]() {
		_interpolateBackToOriginal();
	};
	_interpolator.SetCallback(funcReset);
}

void TimeBounceInterpolator::InterpolateTo(double target, double duration)
{
	_target = target;
	_duration = duration;
	_interpolator.InterpolateTo(_target, _duration);
}

double TimeBounceInterpolator::GetValue() const
{
	return _interpolator.GetValue();
}

void TimeBounceInterpolator::ForceFinish()
{
	_interpolator.ForceFinish();
}

void TimeBounceInterpolator::SetValue(double value)
{
	_interpolator.SetValue(value);
}

void TimeBounceInterpolator::ForceValue(double value)
{
	_interpolator.ForceValue(value);
}
