#include "TimeInterpolatorManager.h"

void TimeFloatInterpolatorManager::RegisterInterpolator(TimeFloatInterpolator* interpolator)
{
	std::lock_guard<std::mutex> lock(mutex);
	interpolators.insert(interpolator);
}

void TimeFloatInterpolatorManager::UnregisterInterpolator(TimeFloatInterpolator* interpolator)
{
	std::lock_guard<std::mutex> lock(mutex);
	interpolators.erase(interpolator);
}

void TimeFloatInterpolatorManager::Update(float dt)
{
	std::lock_guard<std::mutex> lock(mutex);
	for (auto interpolator : interpolators) {
		interpolator->Update(dt);
	}
}
