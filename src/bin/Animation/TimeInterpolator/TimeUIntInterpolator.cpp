#include "TimeUIntInterpolator.h"
#include "TimeInterpolatorManager.h"

TimeUintInterpolator::TimeUintInterpolator(uint32_t initialValue) :
	value(initialValue), target(initialValue), duration(0.0f), elapsedTime(0.0f)
{
	TimeUintInterpolatorManager::RegisterInterpolator(this);
}

TimeUintInterpolator::TimeUintInterpolator()
{
	TimeUintInterpolatorManager::RegisterInterpolator(this);
	this->value = 0;
	this->target = 0;
	this->duration = 0;
	this->elapsedTime = 0;
}

TimeUintInterpolator::~TimeUintInterpolator()
{
	TimeUintInterpolatorManager::UnregisterInterpolator(this);
}

void TimeUintInterpolator::InterpolateTo(uint32_t targetValue, double interpolDuration)
{
	target = targetValue;
	duration = interpolDuration;
	elapsedTime = 0.0f;
}

void TimeUintInterpolator::Update(double dt)
{
	if (elapsedTime < duration) {
		elapsedTime += dt;
		double t = min(elapsedTime / duration, 1.0);
		value = static_cast<uint32_t>(value + (target - value) * t);
	}
}

uint32_t TimeUintInterpolator::GetValue() const
{
	return value;
}
