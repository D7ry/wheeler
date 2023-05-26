#include "TimeFloatInterpolator.h"
#include "TimeInterpolatorManager.h"
TimeFloatInterpolator::TimeFloatInterpolator(double initialValue) :
	value(initialValue), target(initialValue), duration(0.0f), elapsedTime(0.0f)
{
	TimeFloatInterpolatorManager::RegisterInterpolator(this);
}

TimeFloatInterpolator::TimeFloatInterpolator() :
	value(0),
	target(0), duration(0.0f), elapsedTime(0.0f)
{
	TimeFloatInterpolatorManager::RegisterInterpolator(this);
}

TimeFloatInterpolator::~TimeFloatInterpolator()
{
	TimeFloatInterpolatorManager::UnregisterInterpolator(this);
}

void TimeFloatInterpolator::InterpolateTo(double targetValue, double interpolDuration)
{
	target = targetValue;
	duration = interpolDuration;
	elapsedTime = 0.0f;
}

void TimeFloatInterpolator::Update(double dt)
{
	if (elapsedTime < duration) {
		elapsedTime += dt;
		float t = min(elapsedTime / duration, 1.0f);
		value = value + (target - value) * t;
	}
}

double TimeFloatInterpolator::GetValue() const
{
	return value;
}
