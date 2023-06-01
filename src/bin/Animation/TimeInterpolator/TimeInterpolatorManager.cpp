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
	for (auto it = interpolators.begin(); it != interpolators.end();) {
		if ((*it)->Update(dt)) {
			it = interpolators.erase(it);  // interpolator is done, remove it
		} else {
			++it;
		}
	}
	/*ImGui::Begin("INTERPOLATOR DEBUGGING");
	ImGui::Text("%i interpolators present.", interpolators.size());
	ImGui::End();*/
}
