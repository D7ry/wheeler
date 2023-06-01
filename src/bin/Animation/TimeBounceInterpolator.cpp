#include "TimeBounceInterpolator.h"

TimeBounceInterpolator::TimeBounceInterpolator(double originalValue) :
	_original(originalValue)
{
	// Register the callback function to reverse the interpolating direction
	_interpolateBackToOriginal = [this]() {
		_interpolator.InterpolateTo(_original, _duration);
	};
	std::function<void()> funcReset = [this]() { // tell the interpolator to interpolate back to the original value once it's reached the target.
		if (!_isInterpolatingBackToOriginal) { // check whether we're interpolating back to prevent infinite callback.
			_isInterpolatingBackToOriginal = true;
			_interpolateBackToOriginal(); // this will be called once the interpolator reaches the target.
		}
	};
	_interpolator.PushCallback(funcReset);
}

void TimeBounceInterpolator::InterpolateTo(double target, double duration)
{
	_target = target;
	_duration = duration;
	_isInterpolatingBackToOriginal = false;
	_interpolator.InterpolateTo(_target, _duration);
}

double TimeBounceInterpolator::GetValue() const
{
	return _interpolator.GetValue();
}

void TimeBounceInterpolator::ForceFinish()
{
	_interpolator.ForceFinish(false);  // finish the interpolator
	if (!_isInterpolatingBackToOriginal) { // on the forward pass currently
		// calling ForceFinish() on the forward pass gets the interpolator to the destination, we want it back.
		_interpolator.ForceValue(this->_original);
	}
	_isInterpolatingBackToOriginal = false;  // reset the flag
}

void TimeBounceInterpolator::SetValue(double value)
{
	_interpolator.SetValue(value);
}

void TimeBounceInterpolator::ForceValue(double value)
{
	_interpolator.ForceValue(value);
}
