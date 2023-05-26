#pragma once
#include <unordered_set>
#include <mutex>
#include "TimeFloatInterpolator.h"
#include "TimeUIntInterpolator.h"

class TimeFloatInterpolator;
class TimeFloatInterpolatorManager
{
private:
	static inline std::unordered_set<TimeFloatInterpolator*> interpolators;
	static inline std::mutex mutex;

public:
	static void RegisterInterpolator(TimeFloatInterpolator* interpolator);

	static void UnregisterInterpolator(TimeFloatInterpolator* interpolator);

	static void Update(float dt);
};

class TimeUintInterpolatorManager
{
private:
	static inline std::unordered_set<TimeUintInterpolator*> interpolators;
	static inline std::mutex mutex;

public:
	static void RegisterInterpolator(TimeUintInterpolator* interpolator);

	static void UnregisterInterpolator(TimeUintInterpolator* interpolator);

	static void Update(float dt);
};

